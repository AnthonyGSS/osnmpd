/*
 * This file is part of the osnmpd distribution (https://github.com/verrio/osnmpd).
 * Copyright (C) 2016 Olivier Verriest
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stddef.h>
#include <unistd.h>

#include "config.h"
#include "snmp-agent/agent-config.h"
#include "snmp-agent/agent-cache.h"
#include "snmp-agent/mib-tree.h"
#include "snmp-core/utils.h"
#include "snmp-core/snmp-types.h"
#include "snmp-mib/single-level-module.h"
#include "snmp-mib/base/framework-module.h"

#define FRAMEWORK_COMPLIANCE_OID   SNMP_OID_FRAMEWORK_MIB,3,1,1

static SysOREntry framework_or_entry = {
    .or_id = {
        .subid = { FRAMEWORK_COMPLIANCE_OID },
        .len = OID_SEQ_LENGTH(FRAMEWORK_COMPLIANCE_OID)
    },
    .or_descr = "SNMP-FRAMEWORK-MIB - SNMP management architecture MIB",
    .next = NULL
};

enum FrameworkMIBObjects {
    SNMP_FRAMEWORK_ENGINE_ID = 1,
    SNMP_FRAMEWORK_ENGINE_BOOTS = 2,
    SNMP_FRAMEWORK_ENGINE_TIME = 3,
    SNMP_FRAMEWORK_ENGINE_MAX_MESSAGE_SIZE = 4
};

DEF_METHOD(get_scalar, SnmpErrorStatus, SingleLevelMibModule,
        SingleLevelMibModule, int id, SnmpVariableBinding *binding)
{
    switch (id) {
        case SNMP_FRAMEWORK_ENGINE_ID: {
            uint8_t *engine_id;
            size_t engine_id_len = get_engine_id(&engine_id);
            SET_OCTET_STRING_RESULT(binding,
                memdup(engine_id, engine_id_len), engine_id_len);
            break;
        }

        case SNMP_FRAMEWORK_ENGINE_BOOTS: {
            SET_INTEGER_BIND(binding, get_boot_count());
            break;
        }

        case SNMP_FRAMEWORK_ENGINE_TIME: {
            SET_INTEGER_BIND(binding, get_uptime());
            break;
        }

        case SNMP_FRAMEWORK_ENGINE_MAX_MESSAGE_SIZE: {
            SET_INTEGER_BIND(binding, MAX_PDU_SIZE);
            break;
        }
    }

    return NO_ERROR;
}

DEF_METHOD(set_scalar, SnmpErrorStatus, SingleLevelMibModule,
        SingleLevelMibModule, int id, SnmpVariableBinding *binding, int dry_run)
{
    return NOT_WRITABLE;
}

DEF_METHOD(finish_module, void, MibModule, SingleLevelMibModule)
{
    finish_single_level_module(this);
}

MibModule *init_framework_module(void)
{
    SingleLevelMibModule *module = malloc(sizeof(SingleLevelMibModule));
    if (module == NULL) {
        return NULL;
    } else if (init_single_level_module(module, SNMP_FRAMEWORK_ENGINE_ID,
        SNMP_FRAMEWORK_ENGINE_MAX_MESSAGE_SIZE - SNMP_FRAMEWORK_ENGINE_ID + 1,
        LEAF_SCALAR, LEAF_SCALAR, LEAF_SCALAR, LEAF_SCALAR)) {
        free(module);
        return NULL;
    }

    SET_PREFIX(module, SNMP_OID_FRAMEWORK_MIB,2,1);
    SET_OR_ENTRY(module, &framework_or_entry);
    SET_METHOD(module, MibModule, finish_module);
    SET_METHOD(module, SingleLevelMibModule, get_scalar);
    SET_METHOD(module, SingleLevelMibModule, set_scalar);
    return &module->public;
}
