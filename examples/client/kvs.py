# -*- coding: utf-8 -*-
################################################################################
# Copyright 2013-2014 Aerospike, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################

from __future__ import print_function

import aerospike
import sys

from optparse import OptionParser

################################################################################
# Options Parsing
################################################################################

usage = "usage: %prog [options]"

optparser = OptionParser(usage=usage, add_help_option=False)

optparser.add_option(
    "-h", "--host", dest="host", type="string", default="127.0.0.1", metavar="<ADDRESS>",
    help="Address of Aerospike server.")

optparser.add_option(
    "-p", "--port", dest="port", type="int", default=3000, metavar="<PORT>",
    help="Port of the Aerospike server.")

optparser.add_option(
    "--help", dest="help", action="store_true",
    help="Displays this message.")

(options, args) = optparser.parse_args()

if options.help:
    optparser.print_help()
    print()
    sys.exit(1)

################################################################################
# Client Configuration
################################################################################

config = {
    'hosts': [ (options.host, options.port) ]
}

################################################################################
# Application
################################################################################

exitCode = 0

try:

    # ----------------------------------------------------------------------------
    # Connect to Cluster
    # ----------------------------------------------------------------------------

    client = aerospike.client(config).connect()

    # ----------------------------------------------------------------------------
    # Perform Operation
    # ----------------------------------------------------------------------------

    try:

        print('########################################################################')
        print('PUT')
        print('########################################################################')

        for i in range(1000):
            # print 'a'
            # j = igloo
            rec = {
                'i': i,
                's': 'xyz',
                'l': [2,4,8,16,32,None,128,256],
                'm': {'a': 2, 'b': 4, 'c': 8, 'd': 16}
            }
            print(rec)
            client.put(('test','demo',str(i)), rec)

        # print('########################################################################')
        # print('EXISTS')
        # print('########################################################################')

        # for i in range(1,1000):
        #     (key, metadata) = client.exists(('test','demo',i))
        #     print(key, metadata)


        # print('########################################################################')
        # print('GET')
        # print('########################################################################')

        # for i in range(1,1000):
        #     (key, metadata, record) = client.get(('test','demo',i))
        #     print(key, metadata, record)

        # print('########################################################################')
        # print('APPLY')
        # print('########################################################################')

        # for i in range(1,1000):
        #   val1 = client.key('test','demo','key{0}'.format(i)).apply('simple', 'add', ['a', 30000])
        #   print val1

        # print('########################################################################')
        # print('REMOVE')
        # print('########################################################################')

        # for i in range(1,1000):
        #     client.remove(('test','demo',i))

        # print('########################################################################')
        # print('GET')
        # print('########################################################################')

        # for i in range(1,1000):
        #     rec1 = client.get(('test','demo',i))
        #     print(rec1)

    except Exception, eargs:
        print("error: {0}".format(eargs), file=sys.stderr)
        exitCode = 2

    # ----------------------------------------------------------------------------
    # Close Connection to Cluster
    # ----------------------------------------------------------------------------

    client.close()

except Exception, eargs:
    print("error: {0}".format(eargs), file=sys.stderr)
    exitCode = 3

################################################################################
# Exit
################################################################################

sys.exit(exitCode)
