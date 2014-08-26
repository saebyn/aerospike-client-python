/*******************************************************************************
 * Copyright 2013-2014 Aerospike, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#pragma once

#include <Python.h>
#include <stdbool.h>

#include "types.h"

#define TRACE() printf("%s:%d\n",__FILE__,__LINE__)

/*******************************************************************************
 * CLIENT TYPE
 ******************************************************************************/

PyTypeObject * AerospikeClient_Ready(void);

/**
 * Create a new Aerospike client object and connect to the database.
 */
AerospikeClient * AerospikeClient_New(PyObject * self, PyObject * args, PyObject * kwds);

/*******************************************************************************
 * CONNECTION OPERATIONS
 ******************************************************************************/

/**
 * Connect to the database.
 */
PyObject * AerospikeClient_Connect(AerospikeClient * self, PyObject * args, PyObject * kwds);

/**
 * Close the connections to the database.
 */
PyObject * AerospikeClient_Close(AerospikeClient * self, PyObject * args, PyObject * kwds);


/*******************************************************************************
 * KVS OPERATIONS
 ******************************************************************************/

/**
 * Apply a UDF on a record in the database.
 *
 *		client.apply((x,y,z), module, function, args...)
 *
 */
PyObject * AerospikeClient_Apply(AerospikeClient * self, PyObject * args, PyObject * kwds);

/**
 * Check existence of a record in the database.
 *
 *		client.exists((x,y,z))
 *
 */
PyObject * AerospikeClient_Exists(AerospikeClient * self, PyObject * args, PyObject * kwds);

/**
 * Read a record from the database.
 *
 *		client.get((x,y,z))
 *
 */
PyObject * AerospikeClient_Get(AerospikeClient * self, PyObject * args, PyObject * kwds);

/**
 * Reads records from the database.
 *
 *		client.bulk_get((x,y), z1, z2, z3)
 *
 */
PyObject * AerospikeClient_Bulk_Get(AerospikeClient * self, PyObject * args, PyObject * kwds);

/**
 * Write a record in the database.
 *
 *		client.put((x,y,z), ...)
 *
 */
PyObject * AerospikeClient_Put(AerospikeClient * self, PyObject * args, PyObject * kwds);

/**
 * Remove a record from the database.
 *
 *		client.remove((x,y,z))
 *
 */
PyObject * AerospikeClient_Remove(AerospikeClient * self, PyObject * args, PyObject * kwds);


/*******************************************************************************
 * INTENRAL (SHARED) OPERATIONS, FOR COMPATIBILITY W/ OLD API
 ******************************************************************************/

PyObject * AerospikeClient_Apply_Invoke(
	AerospikeClient * self, 
	PyObject * py_key, PyObject * py_module, PyObject * py_function, 
	PyObject * py_arglist, PyObject * py_policy);

PyObject * AerospikeClient_Exists_Invoke(
	AerospikeClient * self, 
	PyObject * py_key, PyObject * py_policy);

PyObject * AerospikeClient_Get_Invoke(
	AerospikeClient * self, 
	PyObject * py_key, PyObject * py_policy);

PyObject * AerospikeClient_Put_Invoke(
	AerospikeClient * self, 
	PyObject * py_key, PyObject * py_bins, PyObject * py_meta, PyObject * py_policy);

PyObject * AerospikeClient_Remove_Invoke(
	AerospikeClient * self, 
	PyObject * py_key, PyObject * py_policy);


/*******************************************************************************
 * KEY OPERATIONS (DEPRECATED)
 ******************************************************************************/

/**
 * This will initialize a key object, which can be used to peform key 
 * operations.
 *
 *		client.key(ns,set,key).put({
 *			"a": 123,
 *			"b": "xyz",
 *			"c": [1,2,3]
 *      })
 *
 *		rec = client.key(ns,set,key).get()
 *
 * @deprecated
 */
AerospikeKey * AerospikeClient_Key(AerospikeClient * self, PyObject * args, PyObject * kwds);

/*******************************************************************************
 * SCAN OPERATIONS
 ******************************************************************************/

/**
 * Performs a `scan` operation. This will initialize a scan object, which can 
 * be used to scan records in specified namespace and/or set.
 *
 * A scan can be executed by calling `foreach`, which will call a callback 
 * each result returned:
 *
 *		def each_result(record):
 *			print record
 *
 *		scan = client.scan(ns,set).foreach(each_result)
 *
 * Alternatively, you can use `results()` which is a generator that will yield a
 * result for each iteration:
 *
 *		for record in client.scan(ns,set).results():
 *			print record
 *
 */
AerospikeScan * AerospikeClient_Scan(AerospikeClient * self, PyObject * args, PyObject * kwds);

/*******************************************************************************
 * QUERY OPERATIONS
 ******************************************************************************/

/**
 * Performs a `query` operation. This will initialize a query object, which 
 * can be used to query records in specified namespace and/or set.
 *
 * A query can be executed by calling `foreach`, which will call a callback 
 * each result returned:
 *
 *		def each_result(result):
 *			print result
 *		
 *		scan = client.query(ns,set).where("a", between(1,100)).foreach(each_result)
 *
 * Alternatively, you can use `results()` which is a generator that will yield a
 * result for each iteration:
 *
 *		for result in client.query(ns,set).where("a", range(1,100)).results():
 *			print result
 *
 */
AerospikeQuery * AerospikeClient_Query(AerospikeClient * self, PyObject * args, PyObject * kwds);

/*******************************************************************************
 * INFO OPERATIONS
 ******************************************************************************/

/**
 * Performs a `info` operation. This will invoke the info request against each
 * node in the cluster. The return value is a dict where the key is the node 
 * name and the value is a tuple of (Error,Response). If an error occurred on
 * the node, the Error will be an object containing details, otherwise it is
 * None. If the request was successful, then the Response will contain the 
 * string response from the node, otherwise it is None.
 *
 *		for node,(err,res) in client.info('statistics').items():
 *			if err == None:
 *				print "{0} - OK: {1}".format(record,res)
 *			else:
 *				print "{0} - ERR: {1}".format(record,err)
 *
 */
PyObject * AerospikeClient_Info(AerospikeClient * self, PyObject * args, PyObject * kwds);


/*******************************************************************************
 * UDF OPERATIONS
 ******************************************************************************/
/**
 * Registers a new UDF.
 *
 *		client.udf_put(policy, filename, udf_type)
 *
 */
PyObject * AerospikeClient_UDF_Put(AerospikeClient * self, PyObject *args, PyObject * kwds);

/**
 * De-registers a UDF.
 *
 *		client.udf_remove(policy, filename)
 *
 */
PyObject * AerospikeClient_UDF_Remove(AerospikeClient * self, PyObject *args, PyObject * kwds);

/**
 * Lists the UDFs
 *
 *		client.udf_list(policy)
 *
 */
PyObject * AerospikeClient_UDF_List(AerospikeClient * self, PyObject *args, PyObject * kwds);


/*******************************************************************************
 * SECONDARY INDEX OPERATIONS
 ******************************************************************************/
/**
 * Create secondary integer index
 *
 *		client.index_integer_create(policy, namespace, set, bin, index_name)
 *
 */
PyObject * AerospikeClient_Index_Integer_Create(AerospikeClient * self, PyObject *args, PyObject * kwds);

/**
 * Create secondary string index
 *
 *		client.index_string_create(policy, namespace, set, bin, index_name)
 *
 */
PyObject * AerospikeClient_Index_String_Create(AerospikeClient * self, PyObject *args, PyObject * kwds);

/**
 * Remove secondary index
 *
 *		client.index_remove(policy, namespace, index_name)
 *
 */
PyObject * AerospikeClient_Index_Remove(AerospikeClient * self, PyObject *args, PyObject * kwds);
