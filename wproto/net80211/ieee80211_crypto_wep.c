/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2002-2008 Sam Leffler, Errno Consulting
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * IEEE 802.11 WEP crypto support.
 */


#include <net80211/ieee80211_var.h>




static	void *wep_attach(struct ieee80211vap *, struct ieee80211_key *);
static	void wep_detach(struct ieee80211_key *);
static	int wep_setkey(struct ieee80211_key *);
static	void wep_setiv(struct ieee80211_key *, uint8_t *);
static	int wep_encap(struct ieee80211_key *, struct mbuf *);
static	int wep_decap(struct ieee80211_key *, struct mbuf *, int);
static	int wep_enmic(struct ieee80211_key *, struct mbuf *, int);
static	int wep_demic(struct ieee80211_key *, struct mbuf *, int);

const struct ieee80211_cipher wep = {
	.ic_name	= "WEP",
	.ic_cipher	= IEEE80211_CIPHER_WEP,
	.ic_header	= IEEE80211_WEP_IVLEN + IEEE80211_WEP_KIDLEN,
	.ic_trailer	= IEEE80211_WEP_CRCLEN,
	.ic_miclen	= 0,
	.ic_attach	= wep_attach,
	.ic_detach	= wep_detach,
	.ic_setkey	= wep_setkey,
	.ic_setiv	= wep_setiv,
	.ic_encap	= wep_encap,
	.ic_decap	= wep_decap,
	.ic_enmic	= wep_enmic,
	.ic_demic	= wep_demic,
};

static	int wep_encrypt(struct ieee80211_key *, struct mbuf *, int hdrlen);
static	int wep_decrypt(struct ieee80211_key *, struct mbuf *, int hdrlen);

struct wep_ctx {
	struct ieee80211vap *wc_vap;	/* for diagnostics+statistics */
	struct ieee80211com *wc_ic;
	uint32_t	wc_iv;		/* initial vector for crypto */
};

/* number of references from net80211 layer */
//static	int nrefs = 0;

static void *
wep_attach(struct ieee80211vap *vap, struct ieee80211_key *k)
{
	struct wep_ctx *ctx;

	ctx = (struct wep_ctx *) IEEE80211_MALLOC(sizeof(struct wep_ctx),
		M_80211_CRYPTO, IEEE80211_M_NOWAIT | IEEE80211_M_ZERO);
	if (ctx == NULL) {
		//vap->iv_stats.is_crypto_nomem++;
		return NULL;
	}

	ctx->wc_vap = vap;
	ctx->wc_ic = vap->iv_ic;
	get_random_bytes(&ctx->wc_iv, sizeof(ctx->wc_iv));
	//nrefs++;			/* NB: we assume caller locking */
	return ctx;
}

static void
wep_detach(struct ieee80211_key *k)
{
	struct wep_ctx *ctx = k->wk_private;

	IEEE80211_FREE(ctx, M_80211_CRYPTO);
	//KASSERT(nrefs > 0, ("imbalanced attach/detach"));
	//nrefs--;			/* NB: we assume caller locking */
}

static int
wep_setkey(struct ieee80211_key *k)
{
	return k->wk_keylen >= 40/NBBY;
}

static void
wep_setiv(struct ieee80211_key *k, uint8_t *ivp)
{
	struct wep_ctx *ctx = k->wk_private;
	struct ieee80211vap *vap = ctx->wc_vap;
	uint32_t iv;
	uint8_t keyid;

	keyid = ieee80211_crypto_get_keyid(vap, k) << 6;

	/*
	 * XXX
	 * IV must not duplicate during the lifetime of the key.
	 * But no mechanism to renew keys is defined in IEEE 802.11
	 * for WEP.  And the IV may be duplicated at other stations
	 * because the session key itself is shared.  So we use a
	 * pseudo random IV for now, though it is not the right way.
	 *
	 * NB: Rather than use a strictly random IV we select a
	 * random one to start and then increment the value for
	 * each frame.  This is an explicit tradeoff between
	 * overhead and security.  Given the basic insecurity of
	 * WEP this seems worthwhile.
	 */

	/*
	 * Skip 'bad' IVs from Fluhrer/Mantin/Shamir:
	 * (B, 255, N) with 3 <= B < 16 and 0 <= N <= 255
	 */
	iv = ctx->wc_iv;
	if ((iv & 0xff00) == 0xff00) {
		int B = (iv & 0xff0000) >> 16;
		if (3 <= B && B < 16)
			iv += 0x0100;
	}
	ctx->wc_iv = iv + 1;

	/*
	 * NB: Preserve byte order of IV for packet
	 *     sniffers; it doesn't matter otherwise.
	 */
#if _BYTE_ORDER == _BIG_ENDIAN
	ivp[0] = iv >> 0;
	ivp[1] = iv >> 8;
	ivp[2] = iv >> 16;
#else
	ivp[2] = iv >> 0;
	ivp[1] = iv >> 8;
	ivp[0] = iv >> 16;
#endif
	ivp[3] = keyid;
}

/*
 * Add privacy headers appropriate for the specified key.
 */
static int
wep_encap(struct ieee80211_key *k, struct mbuf *m)
{
	struct wep_ctx *ctx = k->wk_private;
	struct ieee80211com *ic = ctx->wc_ic;
	struct ieee80211_frame *wh;
	uint8_t *ivp;
	int hdrlen;
	int is_mgmt;

	hdrlen = ieee80211_hdrspace(ic, mtod(m, void *));
	wh = mtod(m, struct ieee80211_frame *);
	is_mgmt = IEEE80211_IS_MGMT(wh);

	/*
	 * Check to see if IV is required.
	 */
	if (is_mgmt && (k->wk_flags & IEEE80211_KEY_NOIVMGT))
		return 1;
	if ((! is_mgmt) && (k->wk_flags & IEEE80211_KEY_NOIV))
		return 1;

	/*
	 * Copy down 802.11 header and add the IV + KeyID.
	 */
	M_PREPEND(m, wep.ic_header, M_NOWAIT);
	if (m == NULL)
		return 0;
	ivp = mtod(m, uint8_t *);
	ovbcopy(ivp + wep.ic_header, ivp, hdrlen);
	ivp += hdrlen;

	wep_setiv(k, ivp);

	/*
	 * Finally, do software encrypt if needed.
	 */
	if ((k->wk_flags & IEEE80211_KEY_SWENCRYPT) &&
	    !wep_encrypt(k, m, hdrlen))
		return 0;

	return 1;
}

/*
 * Add MIC to the frame as needed.
 */
static int
wep_enmic(struct ieee80211_key *k, struct mbuf *m, int force)
{

	return 1;
}

/*
 * Validate and strip privacy headers (and trailer) for a
 * received frame.  If necessary, decrypt the frame using
 * the specified key.
 */
static int
wep_decap(struct ieee80211_key *k, struct mbuf *m, int hdrlen)
{
#ifdef IEEE80211_DEBUG
	struct wep_ctx *ctx = k->wk_private;
	struct ieee80211vap *vap = ctx->wc_vap;
	struct ieee80211_frame *wh;
#endif
	//uint32_t rxs_flags;

#ifdef IEEE80211_DEBUG
	wh = mtod(m, struct ieee80211_frame *);
#endif
#if 0
	rxs_flags = ieee80211_get_rx_params_ptr(m);
  
	if ((rxs_flags & IEEE80211_RX_F_IV_STRIP))
		goto finish;
#endif
	/*
	 * Check if the device handled the decrypt in hardware.
	 * If so we just strip the header; otherwise we need to
	 * handle the decrypt in software.
	 */
	if (((k->wk_flags & IEEE80211_KEY_SWDECRYPT) || (m->m_flags & M_SW_DECRYPT)) &&
	    !wep_decrypt(k, m, hdrlen)) {
		IEEE80211_NOTE_MAC(vap, IEEE80211_MSG_CRYPTO, wh->i_addr2,
		    "%s", "WEP ICV mismatch on decrypt");
		//vap->iv_stats.is_rx_wepfail++;
		return 0;
	}

	/*
	 * Copy up 802.11 header and strip crypto bits.
	 */
	ovbcopy(mtod(m, void *), mtod(m, uint8_t *) + wep.ic_header, hdrlen);
	m_adj(m, wep.ic_header);

//finish:
	/* XXX TODO: do we have to strip this for offload devices? */
	m_adj(m, -wep.ic_trailer);

	return 1;
}

/*
 * Verify and strip MIC from the frame.
 */
static int
wep_demic(struct ieee80211_key *k, struct mbuf *skb, int force)
{
	return 1;
}

#ifndef RISCV
extern const uint32_t crc32_table[256];
#else
    #ifdef OPENCORE
        extern const uint32_t crc32_table[256];
    #else
extern uint32_t iot_crc32_table[256];
#define crc32_table iot_crc32_table
#endif
#endif

static int
wep_encrypt(struct ieee80211_key *key, struct mbuf *m0, int hdrlen)
{
#define S_SWAP(a,b) do { uint8_t t = S[a]; S[a] = S[b]; S[b] = t; } while(0)
#ifdef IEEE80211_DEBUG
	struct wep_ctx *ctx = key->wk_private;
	struct ieee80211vap *vap = ctx->wc_vap;
#endif
	struct mbuf *m = m0;
	uint8_t rc4key[IEEE80211_WEP_IVLEN + IEEE80211_KEYBUF_SIZE];
	uint8_t icv[IEEE80211_WEP_CRCLEN];
	uint32_t i, j, k, crc;
	size_t buflen, data_len;
	uint8_t S[256];
	uint8_t *pos;
	u_int off, keylen;

	//vap->iv_stats.is_crypto_wep++;

	/* NB: this assumes the header was pulled up */
	os_mem_cpy(rc4key, mtod(m, uint8_t *) + hdrlen, IEEE80211_WEP_IVLEN);
	os_mem_cpy(rc4key + IEEE80211_WEP_IVLEN, key->wk_key, key->wk_keylen);

	/* Setup RC4 state */
	for (i = 0; i < 256; i++)
		S[i] = i;
	j = 0;
	keylen = key->wk_keylen + IEEE80211_WEP_IVLEN;
	for (i = 0; i < 256; i++) {
		j = (j + S[i] + rc4key[i % keylen]) & 0xff;
		S_SWAP(i, j);
	}

	off = hdrlen + wep.ic_header;
	data_len = m->m_pkthdr.len - off;

	/* Compute CRC32 over unencrypted data and apply RC4 to data */
	crc = ~0;
	i = j = 0;
	pos = mtod(m, uint8_t *) + off;
	buflen = m->m_len - off;
	for (;;) {
		if (buflen > data_len)
			buflen = data_len;
		data_len -= buflen;
		for (k = 0; k < buflen; k++) {
			crc = crc32_table[(crc ^ *pos) & 0xff] ^ (crc >> 8);
			i = (i + 1) & 0xff;
			j = (j + S[i]) & 0xff;
			S_SWAP(i, j);
			*pos++ ^= S[(S[i] + S[j]) & 0xff];
		}
		if (m->m_next == NULL) {
			if (data_len != 0) {		/* out of data */
				IEEE80211_NOTE_MAC(vap, IEEE80211_MSG_CRYPTO,
				    (uint8_t*)ether_sprintf(mtod(m0,
					struct ieee80211_frame *)->i_addr2),
				    "out of data for WEP (data_len %zu)",
				    data_len);
				/* XXX stat */
				return 0;
			}
			break;
		}
		m = m->m_next;
		pos = mtod(m, uint8_t *);
		buflen = m->m_len;
	}
	crc = ~crc;

	/* Append little-endian CRC32 and encrypt it to produce ICV */
	icv[0] = crc;
	icv[1] = crc >> 8;
	icv[2] = crc >> 16;
	icv[3] = crc >> 24;
	for (k = 0; k < IEEE80211_WEP_CRCLEN; k++) {
		i = (i + 1) & 0xff;
		j = (j + S[i]) & 0xff;
		S_SWAP(i, j);
		icv[k] ^= S[(S[i] + S[j]) & 0xff];
	}
	m_append(m0, IEEE80211_WEP_CRCLEN, icv);

	return 1;
#undef S_SWAP
}

static int
wep_decrypt(struct ieee80211_key *key, struct mbuf *m0, int hdrlen)
{
#define S_SWAP(a,b) do { uint8_t t = S[a]; S[a] = S[b]; S[b] = t; } while(0)
#ifdef IEEE80211_DEBUG
	struct wep_ctx *ctx = key->wk_private;
	struct ieee80211vap *vap = ctx->wc_vap;
#endif
	struct mbuf *m = m0;
	uint8_t rc4key[IEEE80211_WEP_IVLEN + IEEE80211_KEYBUF_SIZE];
	uint8_t icv[IEEE80211_WEP_CRCLEN];
	uint32_t i, j, k, crc;
	size_t buflen, data_len;
	uint8_t S[256];
	uint8_t *pos;
	u_int off, keylen;

	//vap->iv_stats.is_crypto_wep++;

	/* NB: this assumes the header was pulled up */
	os_mem_cpy(rc4key, mtod(m, uint8_t *) + hdrlen, IEEE80211_WEP_IVLEN);
	os_mem_cpy(rc4key + IEEE80211_WEP_IVLEN, key->wk_key, key->wk_keylen);

	/* Setup RC4 state */
	for (i = 0; i < 256; i++)
		S[i] = i;
	j = 0;
	keylen = key->wk_keylen + IEEE80211_WEP_IVLEN;
	for (i = 0; i < 256; i++) {
		j = (j + S[i] + rc4key[i % keylen]) & 0xff;
		S_SWAP(i, j);
	}

	off = hdrlen + wep.ic_header;
	data_len = m->m_pkthdr.len - (off + wep.ic_trailer);

	/* Compute CRC32 over unencrypted data and apply RC4 to data */
	crc = ~0;
	i = j = 0;
	pos = mtod(m, uint8_t *) + off;
	buflen = m->m_len - off;
	for (;;) {
		if (buflen > data_len)
			buflen = data_len;
		data_len -= buflen;
		for (k = 0; k < buflen; k++) {
			i = (i + 1) & 0xff;
			j = (j + S[i]) & 0xff;
			S_SWAP(i, j);
			*pos ^= S[(S[i] + S[j]) & 0xff];
			crc = crc32_table[(crc ^ *pos) & 0xff] ^ (crc >> 8);
			pos++;
		}
		m = m->m_next;
		if (m == NULL) {
			if (data_len != 0) {		/* out of data */
				IEEE80211_NOTE_MAC(vap, IEEE80211_MSG_CRYPTO,
				    mtod(m0, struct ieee80211_frame *)->i_addr2,
				    "out of data for WEP (data_len %zu)",
				    data_len);
				return 0;
			}
			break;
		}
		pos = mtod(m, uint8_t *);
		buflen = m->m_len;
	}
	crc = ~crc;

	/* Encrypt little-endian CRC32 and verify that it matches with
	 * received ICV */
	icv[0] = crc;
	icv[1] = crc >> 8;
	icv[2] = crc >> 16;
	icv[3] = crc >> 24;
	for (k = 0; k < IEEE80211_WEP_CRCLEN; k++) {
		i = (i + 1) & 0xff;
		j = (j + S[i]) & 0xff;
		S_SWAP(i, j);
		/* XXX assumes ICV is contiguous in mbuf */
		if ((icv[k] ^ S[(S[i] + S[j]) & 0xff]) != *pos++) {
			/* ICV mismatch - drop frame */
			return 0;
		}
	}
	return 1;
#undef S_SWAP
}

/*
 * Module glue.
 */
//IEEE80211_CRYPTO_MODULE(wep, 1);
