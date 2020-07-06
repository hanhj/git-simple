#ifndef BDB_H
#define BDB_H

#include "types.h"

int bdb_init ( void );
int bdb_fini ( void );

/**
 * Put key/value into database
 *
 * @param dbname    Database path
 * @param key       Database key
 * @param key_len   Database key length
 * @param value     Database value
 * @param value_len Database value length
 *
 * @return 0: Put success
 *         -1: Put fail
 */

int update_db ( const char *dbname,
                void *key, uint32 key_len,
                void *value, uint32 value_len );
/**
 * Get value from database
 *
 * @param dbname    Database path
 * @param key       Database key
 * @param key_len   Database key length
 * @param value     Database value
 * @param value_len Database value length
 *
 * @return 0: Get value success
 *         -1: Get value fail
 */
int select_db ( const char *dbname,
                void *key, uint32 key_len,
                void *value, uint32 value_len );

#endif
