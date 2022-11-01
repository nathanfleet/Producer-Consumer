# Producer-Consumer

This project demonstrates the producer consumer problem using semaphores and shared memory. The producer produces items and puts them in a table which can hold a maximum of two elements. The producer produces items when there is an open slot in the table, and the consumer consumes items if the table has items to consume.

Compilation:
$ gcc producer.c -pthread -lrt -o producer  
$ gcc consumer.c -pthread -lrt -o consumer  
$ ./producer & ./consumer&  

Example output:  
Producer is ready to produce items.  
Producer is producing an item.  
Slot 0 now contains value 84.  
Consumer is consuming an item.  
Slot 0 contains value: 84. Emptying the slot.  
Producer is producing an item.  
Slot 1 now contains value 78.  
Consumer is consuming an item.  
Slot 1 contains value: 78. Emptying the slot.  
Producer is producing an item.  
Slot 0 now contains value 94.  
Consumer is consuming an item.  
Slot 0 contains value: 94. Emptying the slot.  
