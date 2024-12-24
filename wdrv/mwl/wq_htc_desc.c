#include <net80211/ieee80211_var.h>
#include<linux/hardirq.h>
#include <if_hawkvar.h>
#include "wq_htc_desc.h"
#include "wq_ipc.h"
#include "oss.h"

extern struct ieee80211com *net80211_ic;
void ieee80211_write_reg(struct wq_softc *sc, uint32_t addr, uint32_t val);
void ieee80211_read_reg(struct wq_softc *sc, uint32_t addr, uint8_t *buf, int buf_len);
extern void wq_mac_cmd_in_atomic(struct wq_softc *sc, struct htc_msg_desc *desc);
extern void wq_tx_postpond_set(struct wq_softc *sc, int ind);
extern void wq_tx_postpond_clr(struct wq_softc *sc, int ind);

#ifndef WQ_BUS_TEST
uint32_t WQ_MAC_READ(uint32_t addr)
{
    uint8_t val[4] = {0};

    ieee80211_read_reg((struct wq_softc *)net80211_ic->ic_softc, addr, (uint8_t *)val, sizeof(val));
    return *(uint32_t *)val;
}

void WQ_MAC_WRITE(uint32_t addr, uint32_t val)
{
    ieee80211_write_reg((struct wq_softc *)net80211_ic->ic_softc, addr, val);
    return;
}
#endif


int wq_get_desc_size(void)
{
    return HTC_DATA_DESC_LEN;
}

/* host to target data desc. encap */
struct htc_data_desc *ieee80211_host_desc_gen(struct mbuf *m, struct ieee80211_node *ni)
{
    struct htc_data_desc *data_desc;
    struct ieee80211_frame *wh = (struct ieee80211_frame *) m->m_data;
    uint8_t is_protected = !!(wh->i_fc[1] & IEEE80211_FC1_PROTECTED);
    struct ieee80211_key *k = NULL;
    uint8_t type, subtype;

    /* add encrypt header if needed */
    if (is_protected) {
        k = ieee80211_crypto_encap(ni, m);
        if (k == NULL) {
            DPRINTF(WDRV_DEBUG_INFO, "%s: drop due to key miss\n",__func__);
            dump_bytes("frame", m->m_data, m->m_len);
            return NULL;
        }
        wh = (struct ieee80211_frame *)m->m_data;
    }

    /* reserve space (HTC_DATA_DESC_LEN) for desc. before m->m_data */
    if (!m_prepend(m, HTC_DATA_DESC_LEN, 0)) {
        IOT_ASSERT(0);
    }
    os_mem_set(m->m_data, 0, HTC_DATA_DESC_LEN);
    data_desc = (struct htc_data_desc *)m->m_data;
#if (WQ_CONFIG_64 == 1)
    data_desc->data_start = (uint64_t)m->m_data + HTC_DATA_DESC_LEN;
#else
    data_desc->data_start = (uint32_t)m->m_data + HTC_DATA_DESC_LEN;
#endif
    data_desc->len = m->m_len - HTC_DATA_DESC_LEN; /* len excludes htc header */
    data_desc->sta_idx = ni->ni_mnid;
    data_desc->macid = ni->ni_vap->iv_macid;
    data_desc->info.tx.frame_type = IEEE80211_IS_DATA(wh)? FRAME_TYPE_DATA: FRAME_TYPE_MGMT;
    data_desc->info.tx.is_protect = !!(wh->i_fc[1] & IEEE80211_FC1_PROTECTED);
    data_desc->info.tx.is_mcast = !!IEEE80211_IS_MULTICAST(wh->i_addr1);
    data_desc->info.tx.flags = m->m_flags;
    data_desc->info.tx.qos_node = !!(ni->ni_flags & (IEEE80211_NODE_QOS|IEEE80211_NODE_HT));
    data_desc->info.tx.sw_dur = (m->m_flags & M_FW_SWDUR) ? 1 : 0;
    data_desc->info.tx.no_ack = (m->m_flags & M_FW_NO_ACK) ? 1 : 0;
    data_desc->info.tx.wait_txc = (m->m_flags & M_FW_WAIT_TXC) ? 1 : 0;
    data_desc->info.tx.ether_vtag = M_WME_GETAC(m);
    type = wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK;
    subtype = wh->i_fc[0] & IEEE80211_FC0_SUBTYPE_MASK;
    if ((type == IEEE80211_FC0_TYPE_CTL) && (subtype == IEEE80211_FC0_SUBTYPE_CTS)) {
        data_desc->info.tx.self_cts = 1;
    }
    if ((type == IEEE80211_FC0_TYPE_MGT) && (subtype == IEEE80211_FC0_SUBTYPE_ACTION)) {
        uint8_t *frm;
        const struct ieee80211_action *ia;
        frm = (u_int8_t *)&wh[1];
        ia = (const struct ieee80211_action *) frm;
        if ((ia->ia_category == IEEE80211_ACTION_CAT_BA) && (ia->ia_action == IEEE80211_ACTION_BA_ADDBA_REQUEST)) {
            data_desc->info.tx.is_addba_req = 1;
        }
    }
    data_desc->m = (void *)m;
    if (is_protected) {
        const struct ieee80211_cipher *cip;

        cip = k->wk_cipher;
        data_desc->info.tx.keyid = ieee80211_crypto_get_keyid(ni->ni_vap, k);
        switch (cip->ic_cipher) {
            case IEEE80211_CIPHER_WEP:
                /* reserve ICV */
                data_desc->info.tx.padding = cip->ic_trailer;
                break;
            case IEEE80211_CIPHER_TKIP:
                /* reserve MIC + ICV */
                data_desc->info.tx.padding = cip->ic_miclen + cip->ic_trailer;
                break;
            case IEEE80211_CIPHER_AES_CCM:
                /* reserve MIC */
                data_desc->info.tx.padding = cip->ic_trailer;
                break;
            default:
                IOT_ASSERT(0);
                break;
        }
    }

    return data_desc;
}


/* host to target cmd test */
struct htc_msg_desc *hostmsg_desc_gen(int msg_type, void *data)
{
    struct htc_msg_desc *msg_desc;
    int datalen;
#if (WQ_CONFIG_64 == 1)
    uint64_t *src, *dst;
#else
    uint32_t *src, *dst;
#endif

    switch (msg_type) {
        case MSG_TYPE_REG_READ:
            datalen = sizeof(struct reg_cfg);
            break;
        case MSG_TYPE_REG_WRITE:
            datalen = sizeof(struct reg_cfg);
            break;
        case MSG_TYPE_RC_NODE:
            datalen = sizeof(struct ni_rc_info);
            break;
        case MSG_TYPE_BCN_UPDATE:
            datalen = sizeof(struct beacon_info);
            break;
        case MSG_TYPE_TIM_NOTIFY:
            datalen = sizeof(struct tim_info);
            break;
        case MSG_TYPE_PS_NOTIFY:
            datalen = sizeof(struct ps_info);
            break;
        case MSG_TYPE_TX_AMPDU_NOTIFY:
            datalen = sizeof(struct ampdu_tx_info);
            break;
        case MSG_TYPE_CH_BW_NOTIFY:
            datalen = sizeof(struct ch_bw_info);
            break;
        case MSG_TYPE_NODE_NOTIFY:
            datalen = sizeof(struct node_info);
            break;
        case MSG_TYPE_STATUS_NOTIFY:
            datalen = sizeof(struct status_info);
            break;
        case MSG_TYPE_KEY_NOTIFY:
            datalen = sizeof(struct key_info);
            break;
        case MSG_TYPE_MVAP_NOTIFY:
            datalen = sizeof(struct mvap_info);
            break;
		case MSG_TYPE_FW_INFO:
            datalen = sizeof(struct fw_info);
            break;
        case MSG_TYPE_DEV_INFO:
            datalen = sizeof(struct dev_info);
            break;
        case MSG_TYPE_CALI_INFO:
            datalen = sizeof(struct cali_info);
            break;
        case MSG_TYPE_MP_SET_NOTIFY:
            datalen = sizeof(struct mp_info);
            break;
        case MSG_TYPE_MP_GET_NOTIFY:
            datalen = sizeof(struct mp_info);
            break;
        case MSG_TYPE_AMPDU_AGGR_NOTIFY:
            datalen = sizeof(struct ampdu_agg_info);
            break;
        default:
            IOT_ASSERT(0);
            return NULL;
    };

    msg_desc = mmal_malloc(HTC_MSG_DESC_LEN + datalen);
    IOT_ASSERT(msg_desc);
    msg_desc->msg_idx = msg_type;
    msg_desc->len = datalen;

    src = data;
#if (WQ_CONFIG_64 == 1)
    dst = (uint64_t *)((uint64_t)msg_desc + HTC_MSG_DESC_LEN);
#else
    dst = (uint32_t *)((uint32_t)msg_desc + HTC_MSG_DESC_LEN);
#endif
    os_mem_cpy(dst, src, datalen);
    return msg_desc;
}

void ieee80211_write_reg(struct wq_softc *sc, uint32_t addr, uint32_t val) {
    struct reg_cfg setreg;
    struct htc_msg_desc *desc;
    int ret;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    setreg.addr = addr;
    setreg.value = val;
    desc = hostmsg_desc_gen(MSG_TYPE_REG_WRITE, (void *)&setreg);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                     (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, NULL, 0);
    mmal_free(desc);
}

void ieee80211_read_reg(struct wq_softc *sc, uint32_t addr, uint8_t *buf, int buf_len) {
    struct reg_cfg readreg;
    struct htc_msg_desc *desc;
    uint8_t *rsp_buf;
    uint32_t rsp_len = buf_len + HTC_CFM_DESC_LEN + sizeof(struct reg_cfm);
    int ret;

    rsp_buf = mmal_malloc(rsp_len);
    IOT_ASSERT(rsp_buf);

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    readreg.addr = addr;
    readreg.len = buf_len;
    desc = hostmsg_desc_gen(MSG_TYPE_REG_READ, (void *)&readreg);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                     (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, rsp_buf, rsp_len);
    os_mem_cpy(buf, rsp_buf + HTC_CFM_DESC_LEN + sizeof(struct reg_cfm), buf_len);
    mmal_free(desc);
    mmal_free(rsp_buf);
}

void htc_fw_info(struct wq_softc *sc, struct fw_info_cfm *cfm, uint32_t fw_dbg) {
	struct fw_info fwinfo;
    struct htc_msg_desc *desc;
    uint8_t rsp_buf[HTC_CFM_DESC_LEN + sizeof(struct fw_info_cfm)];
    uint32_t rsp_len = HTC_CFM_DESC_LEN + sizeof(struct fw_info_cfm);
    int ret;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    fwinfo.reserved = fw_dbg;
    desc = hostmsg_desc_gen(MSG_TYPE_FW_INFO, (void *)&fwinfo);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                     (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, rsp_buf, rsp_len);
	//cfm = (struct fw_info_cfm *)(rsp_buf + HTC_CFM_DESC_LEN);
	
    os_mem_cpy(cfm, rsp_buf + HTC_CFM_DESC_LEN, sizeof(struct fw_info_cfm));
	
    mmal_free(desc);
}

void htc_init_node_notify(struct wq_softc *sc, struct ieee80211_node *ni) {
    struct htc_msg_desc *desc;
    struct ni_rc_info rc_info;
    int ret;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    rc_info.ni_mnid = ni->ni_mnid;
    rc_info.ni_capinfo = ni->ni_capinfo;
    rc_info.ni_chw = ni->ni_chw;
    rc_info.ni_htcap = ni->ni_htcap;
    if (sgi_tx_enable == 0) {
        rc_info.ni_htcap &= ~IEEE80211_HTCAP_SHORTGI20;
    }
    memcpy(&rc_info.ni_rates, &(ni->ni_rates), sizeof(struct ieee80211_rateset));
    memcpy(&rc_info.ni_htrates, &(ni->ni_htrates), sizeof(struct ieee80211_htrateset));
    rc_info.ni_erp = ni->ni_erp;

    desc = hostmsg_desc_gen(MSG_TYPE_RC_NODE, (void *)&rc_info);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                     (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, NULL, 0);
    mmal_free(desc);
}

void htc_ps_notify(struct wq_softc *sc, uint8_t macid, int nsta)
{
    struct htc_msg_desc *desc;
    struct ps_info ps;
    int ret;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    ps.macid = macid;
    ps.nsta = nsta;

    desc = hostmsg_desc_gen(MSG_TYPE_PS_NOTIFY, (void *)&ps);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                         (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, NULL, 0);
    mmal_free(desc);

    return;
}

void htc_dev_info_notify(struct wq_softc *sc, uint8_t type, uint8_t *mac_addr, uint8_t param)
{
    struct htc_msg_desc *desc;
    struct dev_info info;
    int ret;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    info.type = type;
    if(mac_addr != NULL)
    {
        os_mem_cpy(info.ap_mac, mac_addr, IEEE80211_ADDR_LEN);
    }
    info.param = param;

    desc = hostmsg_desc_gen(MSG_TYPE_DEV_INFO, (void *)&info);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                         (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, NULL, 0);
    mmal_free(desc);

    return;
}

void htc_tim_notify(struct wq_softc *sc, uint8_t macid, uint8_t pm, uint16_t aid)
{
    struct htc_msg_desc *desc;
    struct tim_info sta;

    sta.macid = macid;
    sta.pm = pm;
    sta.aid = aid;

    desc = hostmsg_desc_gen(MSG_TYPE_TIM_NOTIFY, (void *)&sta);

    /* queue this cmd and do in work */
    wq_mac_cmd_in_atomic(sc, desc);

    return;
}

void htc_beacon_update_notify(struct wq_softc *sc, uint8_t macid, uint8_t mnid, struct mbuf *m)
{
    struct htc_msg_desc *desc;
    struct beacon_info bcn;
    int ret;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    IOT_ASSERT(m->m_len < MAX_BCN_LEN);
    bcn.macid = macid;
    bcn.mnid = mnid;
    bcn.bcn_len = m->m_len;
    memcpy(bcn.bcn_data, m->m_data, m->m_len);

    desc = hostmsg_desc_gen(MSG_TYPE_BCN_UPDATE, (void *)&bcn);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                         (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, NULL, 0);
    mmal_free(desc);

    return;
}

void htc_tx_ampdu_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t mnid, uint8_t ac, uint8_t code)
{
    struct htc_msg_desc *desc;
    struct ampdu_tx_info ampdu;

    ampdu.type= msg_type;
    ampdu.mnid= mnid;
    ampdu.ac= ac;
    ampdu.code= code;

    desc = hostmsg_desc_gen(MSG_TYPE_TX_AMPDU_NOTIFY, (void *)&ampdu);

    /* queue this cmd and do in work */
    wq_mac_cmd_in_atomic(sc, desc);

    return;
}

void htc_ampdu_agg_set_notify(struct wq_softc *sc, uint8_t agg_num)
{
    int ret = -1;
    struct htc_msg_desc *desc;
    struct ampdu_agg_info ampdu;

    if (agg_num < 1 || agg_num > 12) {
        iot_printf("%s, aggregation num is out of range!\n", __func__);
        return;
    }

    ampdu.type= AMPDU_AGG_SET;
    ampdu.num = agg_num;

    desc = hostmsg_desc_gen(MSG_TYPE_AMPDU_AGGR_NOTIFY, (void *)&ampdu);

    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                         (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, NULL, 0);

    mmal_free(desc);

    return;
}

uint8_t htc_get_ampdu_aggr_num(struct wq_softc *sc)
{
    struct htc_msg_desc *desc = NULL;
    struct ampdu_agg_info ampdu;
    struct ampdu_agg_cfg *agg_cfg = NULL;
    uint8_t rsp_len = 0;
    uint8_t *rsp = NULL;
    int ret = -1;

    ampdu.type = AMPDU_AGG_GET;
    ampdu.num = 0;

    rsp_len = sizeof(struct ampdu_agg_cfg) + HTC_CFM_DESC_LEN;
    rsp = mmal_malloc(rsp_len);
    IOT_ASSERT(rsp);
    desc = hostmsg_desc_gen(MSG_TYPE_AMPDU_AGGR_NOTIFY, (void *)&ampdu);

    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                         (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, rsp, rsp_len);

    mmal_free(desc);
    if (rsp) {
        agg_cfg = (struct ampdu_agg_cfg *)(rsp + HTC_CFM_DESC_LEN);
        if (agg_cfg->result == CFM_SUCC) {
            ret = agg_cfg->num;
        }
        mmal_free(rsp);
    }
    return ret;
}

int htc_ch_bw_notify(struct wq_softc *sc, uint8_t ch_num, uint16_t freq, int8_t power, uint8_t sw_ch_cali)
{
    struct ieee80211com *ic = &sc->sc_ic;
    struct htc_msg_desc *desc = NULL;
    struct ch_bw_info ch_bw;
    uint8_t *rsp = NULL;
    int ret = -1;
    uint32_t rsp_len = 0;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    ch_bw.ic_ieee = ch_num;
    ch_bw.ic_freq = freq;
    ch_bw.ic_maxregpower = power;
    ch_bw.sw_ch_cali = sw_ch_cali;

    rsp_len = sizeof(struct ch_bw_cfm) + HTC_CFM_DESC_LEN;
    rsp = mmal_malloc(rsp_len);
    IOT_ASSERT(rsp);
    ((struct ch_bw_cfm *)(rsp + HTC_CFM_DESC_LEN))->result = CFM_SUCC;
    ((struct ch_bw_cfm *)(rsp + HTC_CFM_DESC_LEN))->ic_ieee = ch_bw.ic_ieee;

    desc = hostmsg_desc_gen(MSG_TYPE_CH_BW_NOTIFY, (void *)&ch_bw);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                         (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, rsp, rsp_len);

    mmal_free(desc);

    /* handle response from fw */
    if (rsp) {
        ic->ic_cfm_result = ((struct ch_bw_cfm *)(rsp + HTC_CFM_DESC_LEN))->result;
        if (ic->ic_cfm_result == CFM_SUCC) {
            ic->ic_rf_chan = ((struct ch_bw_cfm *)(rsp + HTC_CFM_DESC_LEN))->ic_ieee;
            IOT_ASSERT(ch_num == ic->ic_rf_chan);
            ret = 0;
        } else {
            IOT_ASSERT(0);
        }

        mmal_free(rsp);
    }

    return ret;
}

uint8_t htc_node_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t mnid, uint8_t erp)
{
    struct htc_msg_desc *desc;
    struct node_info node;
    int ret;
    uint8_t *rsp = NULL, rsp_id;
    uint32_t rsp_len = 0;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    node.type= msg_type;
    node.erp = erp;
    node.mnid = mnid;

    /* retrieve mnid from fw when NODE_ALLOC */
    if (msg_type == NODE_ALLOC) {
        rsp_len = sizeof(struct node_cfm) + HTC_CFM_DESC_LEN;
        rsp = mmal_malloc(rsp_len);
        IOT_ASSERT(rsp);
    }

    desc = hostmsg_desc_gen(MSG_TYPE_NODE_NOTIFY, (void *)&node);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                     (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, rsp, rsp_len);
    mmal_free(desc);

    /* handle response from fw */
    if (rsp) {
        IOT_ASSERT(msg_type == NODE_ALLOC);
        rsp_id = ((struct node_cfm *)(rsp + HTC_CFM_DESC_LEN))->mnid;
        mmal_free(rsp);
        if (ret == 0) {
            return rsp_id;
        }
    }
    return WQ_NODE_INVALID;
}

int htc_status_notify(struct wq_softc *sc, uint8_t msg_type)
{
    struct htc_msg_desc *desc;
    struct status_info status;
    int ret;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    status.type= msg_type;

    desc = hostmsg_desc_gen(MSG_TYPE_STATUS_NOTIFY, (void *)&status);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                     (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, NULL, 0);
    mmal_free(desc);

    return ret;
}

void htc_key_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t mnid, uint8_t *keyval, uint8_t *mac)
{
    struct htc_msg_desc *desc;
    struct key_info key;
    int ret;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    IOT_ASSERT(sizeof(WQ_HAL_KEYVAL) < MAX_KEYINFO_LEN);

    key.type= msg_type;
    key.mnid = mnid;
    os_mem_cpy(key.key_info, keyval, sizeof(WQ_HAL_KEYVAL));
    os_mem_cpy(key.mac, mac, IEEE80211_ADDR_LEN);

    desc = hostmsg_desc_gen(MSG_TYPE_KEY_NOTIFY, (void *)&key);
    wq_tx_postpond_set(sc, WQ_POSTPOND_KEY);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                     (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, NULL, 0);
    wq_tx_postpond_clr(sc, WQ_POSTPOND_KEY);
    mmal_free(desc);

    return;
}

void htc_mvap_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t macid, uint8_t opmode)
{
    struct htc_msg_desc *desc;
    struct mvap_info info;
    int ret;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    info.type = msg_type;
    info.macid = macid;
    info.opmode = opmode;
    desc = hostmsg_desc_gen(MSG_TYPE_MVAP_NOTIFY, (void *)&info);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                     (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, NULL, 0);
    mmal_free(desc);
    return;
}

void htc_cali_info(struct wq_softc *sc, uint8_t *cali_data, uint16_t cali_len) {
    struct htc_msg_desc *desc;
    struct cali_info cali;
    int ret;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    cali.cali_len = cali_len;
    memcpy(cali.cali_data, cali_data, cali_len);

    desc = hostmsg_desc_gen(MSG_TYPE_CALI_INFO, (void *)&cali);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                         (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, NULL, 0);
    mmal_free(desc);
}

void htc_mp_set_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t *param, uint16_t param_len)
{
    struct htc_msg_desc *desc;
    struct mp_info mp;
    int ret;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    IOT_ASSERT(param_len <= MAX_MP_PARAM_LEN);

    mp.type = msg_type;
    mp.param_len = param_len;
    memcpy(mp.param, param, param_len);

    desc = hostmsg_desc_gen(MSG_TYPE_MP_SET_NOTIFY, (void *)&mp);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                         (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, NULL, 0);
    mmal_free(desc);
}

int htc_mp_get_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t *param, uint16_t param_len, uint8_t *buf, uint16_t buf_len)
{
    struct htc_msg_desc *desc;
    struct mp_info mp;
    uint8_t *rsp_buf;
    uint32_t rsp_len = HTC_CFM_DESC_LEN + sizeof(struct mp_get_cfm) + buf_len;
    int ret;
    char result;

    if (in_atomic()) {
        IOT_ASSERT(0);
    }

    rsp_buf = mmal_malloc(rsp_len);
    mp.type = msg_type;
    if (param_len) {
        os_mem_cpy(mp.param, param, param_len);
    }
    mp.param_len = param_len;
    mp.rsp_len = buf_len;

    desc = hostmsg_desc_gen(MSG_TYPE_MP_GET_NOTIFY, (void *)&mp);
    ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, WQ_IPC_CMD,
                     (uint8_t *)desc, desc->len + HTC_MSG_DESC_LEN, rsp_buf, rsp_len);

    result = rsp_buf[HTC_CFM_DESC_LEN];
    if ((ret == 0) && (result == CFM_SUCC)) {
        os_mem_cpy(buf, rsp_buf + HTC_CFM_DESC_LEN + sizeof(struct mp_get_cfm), buf_len);
    } else {
        ret = -1;
    }
    mmal_free(desc);
    mmal_free(rsp_buf);

    return ret;
}

