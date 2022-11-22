// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) Jim Ma */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/if_vlan.h>
#include <linux/aer.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/pm_runtime.h>
#include <linux/prefetch.h>
#include <net/pkt_sched.h>

#include <net/ipv6.h>

static inline unsigned char *skb_checksum_start(const struct sk_buff *skb)
{
        return skb->head + skb->csum_start;
}

static inline void csum_replace_by_diff(__sum16 *sum, __wsum diff)
{
        *sum = csum_fold(csum_add(diff, ~csum_unfold(*sum)));
}

static inline void net_prefetch(void *p)
{
        prefetch(p);
#if L1_CACHE_BYTES < 128
        prefetch((u8 *)p + L1_CACHE_BYTES);
#endif
}

static inline bool dev_page_is_reusable(struct page *page)
{
        return likely(page_to_nid(page) == numa_mem_id() &&
                      !page_is_pfmemalloc(page));
}

/**
 * refcount_read - get a refcount's value
 * @r: the refcount
 *
 * Return: the refcount's value
 */
static inline unsigned int refcount_read(atomic_t *r)
{
        return atomic_read(r);
}

// static inline __must_check bool __refcount_sub_and_test(int i, atomic_t *r, int *oldp)
// {
//         // int old = atomic_fetch_sub_release(i, r);

//         // if (oldp)
//         //         *oldp = old;

//         // if (old == i) {
//         //         smp_acquire__after_ctrl_dep();
//         //         return true;
//         // }

//         // if (unlikely(old < 0 || old - i < 0))
//         //         refcount_warn_saturate(r, REFCOUNT_SUB_UAF);

//         return false;
// }

/**
 * refcount_sub_and_test - subtract from a refcount and test if it is 0
 * @i: amount to subtract from the refcount
 * @r: the refcount
 *
 * Similar to atomic_dec_and_test(), but it will WARN, return false and
 * ultimately leak on underflow and will fail to decrement when saturated
 * at REFCOUNT_SATURATED.
 *
 * Provides release memory ordering, such that prior loads and stores are done
 * before, and provides an acquire ordering on success such that free()
 * must come after.
 *
 * Use of this function is not recommended for the normal reference counting
 * use case in which references are taken and released one at a time.  In these
 * cases, refcount_dec(), or one of its variants, should instead be used to
 * decrement a reference count.
 *
 * Return: true if the resulting refcount is 0, false otherwise
 */
// static inline __must_check bool refcount_sub_and_test(int i, atomic_t *r)
// {
//         return __refcount_sub_and_test(i, r, NULL);
// }

// static inline __must_check bool __refcount_dec_and_test(atomic_t *r, int *oldp)
// {
//         return __refcount_sub_and_test(1, r, oldp);
// }

/**
 * refcount_dec_and_test - decrement a refcount and test if it is 0
 * @r: the refcount
 *
 * Similar to atomic_dec_and_test(), it will WARN on underflow and fail to
 * decrement when saturated at REFCOUNT_SATURATED.
 *
 * Provides release memory ordering, such that prior loads and stores are done
 * before, and provides an acquire ordering on success such that free()
 * must come after.
 *
 * Return: true if the resulting refcount is 0, false otherwise
 */
// static inline __must_check bool refcount_dec_and_test(atomic_t *r)
// {
//         return __refcount_dec_and_test(r, NULL);
// }

/**
 * skb_unref - decrement the skb's reference count
 * @skb: buffer
 *
 * Returns true if we can free the skb.
 */
// static inline bool skb_unref(struct sk_buff *skb)
// {
//         if (unlikely(!skb))
//                 return false;
//         if (likely(atomic_read(&skb->users) == 1))
//                 smp_rmb();
//         else if (likely(!refcount_dec_and_test(&skb->users)))
//                 return false;

//         return true;
// }

// static void skb_release_all(struct sk_buff *skb)
// {
//         skb_release_head_state(skb);
//         if (likely(skb->head))
//                 skb_release_data(skb);
// }

// static void napi_consume_skb(struct sk_buff *skb, int budget)
// {
//         /* Zero budget indicate non-NAPI context called us, like netpoll */
//         if (unlikely(!budget)) {
//                 dev_consume_skb_any(skb);
//                 return;
//         }

//         // lockdep_assert_in_softirq();

//         if (!skb_unref(skb))
//                 return;

//         /* if reaching here SKB is ready to free */
//         trace_consume_skb(skb);

//         /* if SKB is a clone, don't handle this case */
//         if (skb->fclone != SKB_FCLONE_UNAVAILABLE) {
//                 __kfree_skb(skb);
//                 return;
//         }

//         skb_release_all(skb);
//         napi_skb_cache_put(skb);
// }

static inline int
pci_request_mem_regions(struct pci_dev *pdev, const char *name)
{
        return pci_request_selected_regions(pdev,
                            pci_select_bars(pdev, IORESOURCE_MEM), name);
}

static inline void
pci_release_mem_regions(struct pci_dev *pdev)
{
        return pci_release_selected_regions(pdev,
                            pci_select_bars(pdev, IORESOURCE_MEM));
}

static inline bool ethtool_convert_link_mode_to_legacy_u32(u32 *legacy_u32,
                const unsigned long *src)
{
        bool retval = true;

        /* TODO: following test will soon always be true */
        if (__ETHTOOL_LINK_MODE_MASK_NBITS > 32) {
                __ETHTOOL_DECLARE_LINK_MODE_MASK(ext);

                bitmap_zero(ext, __ETHTOOL_LINK_MODE_MASK_NBITS);
                bitmap_fill(ext, 32);
                bitmap_complement(ext, ext, __ETHTOOL_LINK_MODE_MASK_NBITS);
                if (bitmap_intersects(ext, src,
                                      __ETHTOOL_LINK_MODE_MASK_NBITS)) {
                        /* src mask goes beyond bit 31 */
                        retval = false;
                }
        }
        *legacy_u32 = src[0];
        return retval;
}

static inline int page_ref_sub_and_test(struct page *page, int nr)
{
	int ret = atomic_sub_and_test(nr, &page->_count);
	return ret;
}

static inline void __page_frag_cache_drain(struct page *page, unsigned int count)
{
	if (page_ref_sub_and_test(page, count)) {
		unsigned int order = compound_order(page);

                // TODO optimize with free_unref_page
		// if (order == 0)
		// 	free_unref_page(page);
		// else
			__free_pages(page, order);
	}
}