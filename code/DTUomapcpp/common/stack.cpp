/*
 * stack.cpp
 *
 *  Created on: 2013-8-14
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdlib.h>
//#include <mem.h>
#include <string.h>
#include "stack.h"
#include "fa_fft_cal.h"
#include "evmomapl138_timer.h"
Stack::Stack(){
	Node*node	;
	//printf("Stack creak..\n");
	node=(Node*)malloc(sizeof(Node));
	node->pre=NULL;
	node->next=NULL;
	head=node;
	current_count=0;
}

int8 Stack::Push ( MessageData<BaseDataType> msg )
{
    Node*node ;
    node = ( Node* ) malloc ( sizeof ( Node ) );

    if ( node == NULL ) return 0;

    node->next = NULL;

    node->pre = NULL;

    memcpy ( &node->data, &msg, sizeof ( MessageData<BaseDataType> ) );

    node->data.Data = ( BaseDataType* ) malloc ( msg.Size + 8 );

    memcpy ( node->data.Data, msg.Data, msg.Size + 8 );

    if ( head->next != NULL ) {
        node->next = head->next;
        node->next->pre = node;
    }

    node->pre = head;

    head->next = node;

    if ( current_count == 0 )
        last = node;

    current_count++;

    return 1;
}
int8 Stack::Push ( MessageData<BaseDataType> msg ,char *data)
{
    Node*node ;
    node = ( Node* ) malloc ( sizeof ( Node ) );

    if ( node == NULL ) return 0;

    node->next = NULL;

    node->pre = NULL;

    memcpy ( &node->data, &msg, sizeof ( MessageData<BaseDataType> ) );

    node->data.Data = ( BaseDataType* ) malloc ( msg.Size + 8 );

    memcpy ( & ( ( ( DataType1* ) ( node->data.Data ) )->Data ), data, msg.Size  );
 
    if ( head->next != NULL ) {
        node->next = head->next;
        node->next->pre = node;
    }

    node->pre = head;

    head->next = node;

    if ( current_count == 0 )
        last = node;

    current_count++;

    return 1;
}


Node*  Stack::Pop ( int8 del_flag )
{
    Node*node;

    if ( current_count == 0 ) return NULL;

    node = last;

    if ( del_flag ) {
        node->pre->next = NULL;
        last = node->pre;
        current_count--;
//free(node->data.Data);
        return node;
    }

    return node;
}

void  Stack::Flush()
{
    Node* node;

    while ( current_count != 0 ) {
        node = Pop ( 1 );
        free ( node->data.Data );
        free ( node );
    }
}
