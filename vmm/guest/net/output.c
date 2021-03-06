#include "ns.h"
#define VMX_VMCALL_NETSEND 0x4

extern union Nsipc nsipcbuf;

int32_t net_host_send(char *pg, int len) {
    // LAB 8: Your code here.

    if (pg == NULL)
        pg = (void*) UTOP;

    uint64_t addr = (uint64_t)pg;                                                                                                                                   

    if( (vpml4e[VPML4E(addr)] & PTE_P)   &&   (vpde[VPDPE(addr)] & PTE_P)                                                                                     
            &&  (vpd[VPD(addr)] & PTE_P)  &&  (vpt[VPN(addr)] & PTE_P)  )
    {
        pg = (void *) PTE_ADDR( vpt[VPN(addr)] );
    }

    int ret =  vm_call( VMX_VMCALL_NETSEND, (uint64_t) pg , (uint64_t) len, 0, 0, 0 );
    return ret;
}

    void
output(envid_t ns_envid)
{
    binaryname = "ns_output";
    char *buf=(char *)malloc(sizeof(char) * 2048);
    // LAB 6: Your code here:
    // 	- read a packet from the network server
    //	- send the packet to the device driver
    int r;
    int len = 0;
    while(1) {
        r = sys_ipc_recv(&nsipcbuf);
        if ( (thisenv->env_ipc_from != ns_envid) || (thisenv->env_ipc_value != NSREQ_OUTPUT)) {
            continue;
        }
        len=nsipcbuf.pkt.jp_len;
        memmove(buf, nsipcbuf.pkt.jp_data, nsipcbuf.pkt.jp_len); 
        while ((r = net_host_send(buf, len)) != 0);
    }

}
