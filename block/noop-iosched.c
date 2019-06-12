/*
 * elevator noop
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

struct noop_data {
	struct list_head queue;
};

static void noop_merged_requests(struct request_queue *q, struct request *rq,
				 struct request *next)
{
	list_del_init(&next->queuelist);
}


static int noop_dispatch(struct request_queue *q, int force)
{
	struct noop_data *nd = q->elevator->elevator_data;
	struct request *rq;

	rq = list_first_entry_or_null(&nd->queue, struct request, queuelist);  //获取队首的请求
	if (rq && rq->bio) {
	    struct inode *inode = NULL;
	    struct bio_vec *bvec;
	    int i;
	    int rw = -1;
	    int data_type = -1;
	    struct bio *bio1;
		bio1 = rq->bio;
	    rw = op_is_write(bio1->bi_opf)? 0:1;

 		if (req_op(rq) == REQ_OP_DISCARD)
 		{
 			printk(
 				KERN_ALERT "trim %lu 0 %u %lu 2 -1 -1 0 0\n",
 				rq->start_time,
 				rq->__sector,
 				rq->__data_len);
 		}

 		if (rw == 0) {
 			bio1 = rq->bio;
	   		data_type = bio1->type;
	 	    if (rq->__sector <= 233472) {
		    	printk(
		    	KERN_ALERT "metadata_write %lu 0 %u %lu 0 -1 -1 0 0\n",
		    	rq->start_time,
	        	rq->__sector,
		    	rq->__data_len);
		    }
		    else if (data_type == 0 || data_type == 1) {
		        bio1 = rq->bio;
			    printk(
			    KERN_ALERT "data_write %lu 0 %u %lu 0 -1 -1 %d %d\n",
			    rq->start_time,
		        rq->__sector,
			    rq->__data_len,
			    bio1->type,
			    bio1->temp);
		        if (bio1->bi_io_vec && bio1->bi_io_vec->bv_page && bio1->bi_vcnt > 0) {
		            bio_for_each_segment_all(bvec, bio1, i){	
		                struct page *page = bvec->bv_page;
			     		if (page->mapping) {
			         		inode = page->mapping->host;       
	                        if (inode) {
			   	       			printk(
				       			KERN_ALERT "%d %d\n",
				      	 			//rq->start_time,
				       			inode->i_ino,
	                            page->index);
				       				//bio1->bi_iter.bi_size,
				       				//bio1->bi_iter.bi_sector,
	                            	//bio1->temp,
	                            	//bio1->type);
			            		                        	
	                        }			
			        	}        
	            	}
	           	}
			 	while(bio1 != rq->biotail) {
			        bio1 = bio1->bi_next;
		            if (bio1->bi_io_vec && bio1->bi_io_vec->bv_page && bio1->bi_vcnt > 0) {
				   		bio_for_each_segment_all(bvec, bio1, i) {	
		     		       	struct page *page = bvec->bv_page;
				       		if (page->mapping) {
					   			inode = page->mapping->host;       
	                        	if (inode) {
						   			printk(
						   			KERN_ALERT "%d %d\n",
						   			//rq->start_time,
						   			inode->i_ino,
	                            	page->index);
						   			//bio1->bi_iter.bi_size,
						   			//bio1->bi_iter.bi_sector,
	                            	//bio1->temp,
	                            	//bio1->type);                        	
	                            }			
			                }        
	                    }
	                } 
	            }
		    } 
		}
		if (rw == 1) {
	 		bio1 = rq->bio;
			if (rq->__sector <= 233472) {
	    		printk(
	    		KERN_ALERT "metadata_read %lu 0 %u %lu 1 -1 -1 0 0\n",
	    		rq->start_time,
        		rq->__sector,
	    		rq->__data_len);
	    	}	
	    	else {	
		    	bio1 = rq->bio;
			    printk(
			    KERN_ALERT "data_read %lu 0 %u %lu 1 -1 -1 %d %d\n",
			    rq->start_time,
		        rq->__sector,
			    rq->__data_len,
			    bio1->type,
			    bio1->temp);
		        if (bio1->bi_io_vec && bio1->bi_io_vec->bv_page && bio1->bi_vcnt > 0) {
		            bio_for_each_segment_all(bvec, bio1, i){	
		                struct page *page = bvec->bv_page;
			     		if (page->mapping) {
			         		inode = page->mapping->host;       
	                        if (inode) {
			   	       			printk(
				       			KERN_ALERT "%d %d\n",
				      	 		//rq->start_time,
				       			inode->i_ino,
	                            page->index);
				       			//bio1->bi_iter.bi_size,
				       			//bio1->bi_iter.bi_sector,
	                            //bio1->temp,
	                            //bio1->type);                       	
	                        }			
			        	}        
	            	}
	            }
			 	while(bio1 != rq->biotail) {
			        bio1 = bio1->bi_next;
	               	if (bio1->bi_io_vec && bio1->bi_io_vec->bv_page && bio1->bi_vcnt > 0) {
			   			bio_for_each_segment_all(bvec, bio1, i) {	
	     		       		struct page *page = bvec->bv_page;
			       			if (page->mapping) {
				   			inode = page->mapping->host;       
	                            if (inode) {
					   				printk(
					   				KERN_ALERT "%d %d\n",
					   				//rq->start_time,
					   				inode->i_ino,
	                                page->index);
					   				//bio1->bi_iter.bi_size,
					   				//bio1->bi_iter.bi_sector,
	                                //bio1->temp,
	                                //bio1->type);                        	
	                            }			
		                    }        
	                    }
	                } 
			    } 
			}    	    	
		}			
		list_del_init(&rq->queuelist);		                  //从队列中删除
		elv_dispatch_sort(q, rq);                             //将rq插入request_queue
		return 1;
    }
	return 0;
}




static void noop_add_request(struct request_queue *q, struct request *rq)
{
	struct noop_data *nd = q->elevator->elevator_data;

	list_add_tail(&rq->queuelist, &nd->queue);
}

static struct request *
noop_former_request(struct request_queue *q, struct request *rq)
{
	struct noop_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.prev == &nd->queue)
		return NULL;
	return list_prev_entry(rq, queuelist);
}

static struct request *
noop_latter_request(struct request_queue *q, struct request *rq)
{
	struct noop_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.next == &nd->queue)
		return NULL;
	return list_next_entry(rq, queuelist);
}

static int noop_init_queue(struct request_queue *q, struct elevator_type *e)
{
	struct noop_data *nd;
	struct elevator_queue *eq;

	eq = elevator_alloc(q, e);
	if (!eq)
		return -ENOMEM;

	nd = kmalloc_node(sizeof(*nd), GFP_KERNEL, q->node);
	if (!nd) {
		kobject_put(&eq->kobj);
		return -ENOMEM;
	}
	eq->elevator_data = nd;

	INIT_LIST_HEAD(&nd->queue);

	spin_lock_irq(q->queue_lock);
	q->elevator = eq;
	spin_unlock_irq(q->queue_lock);
	return 0;
}

static void noop_exit_queue(struct elevator_queue *e)
{
	struct noop_data *nd = e->elevator_data;

	BUG_ON(!list_empty(&nd->queue));
	kfree(nd);
}

static struct elevator_type elevator_noop = {
	.ops.sq = {
		.elevator_merge_req_fn		= noop_merged_requests,
		.elevator_dispatch_fn		= noop_dispatch,
		.elevator_add_req_fn		= noop_add_request,
		.elevator_former_req_fn		= noop_former_request,
		.elevator_latter_req_fn		= noop_latter_request,
		.elevator_init_fn		= noop_init_queue,
		.elevator_exit_fn		= noop_exit_queue,
	},
	.elevator_name = "noop",
	.elevator_owner = THIS_MODULE,
};

static int __init noop_init(void)
{
	return elv_register(&elevator_noop);
}

static void __exit noop_exit(void)
{
	elv_unregister(&elevator_noop);
}

module_init(noop_init);
module_exit(noop_exit);


MODULE_AUTHOR("Jens Axboe");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("No-op IO scheduler");
