local util          = require "util"
local mysql         = require "mysql"
local foreach       = util.foreach
local type          = type
local next          = next
local pairs         = pairs
local concat        = table.concat
local tonumber      = tonumber
local tostring      = tostring
local quote_sql_str = ngx.quote_sql_str

--==================================================================================================
-- AUTH : yuanfanbin
-- BEGIN: MYSQL 语句生成器(openresty(ngx+lua)中使用)
--==================================================================================================
local function process_where(where_params)
    --[[ where条件处理 ]]
    if not (type(where_params) == 'table' and next(where_params)) then 
        return ' 1=1 '      -- 未提供条件，默认ALL
    end
    local where = {}
    for k, v in pairs(where_params) do
        if type(v) == 'string' then                             -- 可接受string
            v = (v ~= '') and quote_sql_str(v) or "''"
            where[#where + 1] = k .. '=' .. v

        elseif type(v) == 'boolean' then                        -- 可接受boolean
            v = (v) and 'true' or 'false'
            where[#where + 1] = k .. '=' .. v

        elseif type(v) == 'number' then                         -- 可接受int32
            where[#where + 1] = k .. '=' .. tonumber(v)

        elseif type(k) == 'string' and type(v) == 'table' then  -- key=>string, value=>table
            if k == '=' then    -- ['='] = {'t1.id', 't2.id'}
                where[#where + 1] = v[1] .. '=' .. v[2]
            else
                v = concat(foreach(v, quote_sql_str), ',')
                where[#where + 1] = k .. ' IN ( ' .. v .. ' )'
            end

        elseif type(k) == 'table' and type(v) == 'table' then   -- key=>table, value=>table{ table }
            local ins = {}
            foreach(v, function(_v) ins[#ins + 1] = concat{'(', concat(_v, ','), ')'} end)
            where[#where + 1] = concat{'(', concat(k, ','), ')'} ..  ' IN ' ..  concat{'(', concat(ins, ','), ')'}
        end
    end
    return concat(where, ' AND ')
end

local function process_other(other_params)
    --[[ LIMIT, ORDER 条件处理 ]]
    if not (type(other_params) == 'table' and next(other_params)) then
        return ''
    end
    local other = ''
    if other_params['ORDER'] then                               -- 可接受 order 排序方式[DESC/ASC]
        other = concat{other, ' ORDER BY ', other_params['ORDER']}
    end
    if other_params['LIMIT'] then                               -- 可接受 limit 数字(int32)
        other = concat{other, ' LIMIT ', tonumber(other_params['LIMIT'])}
    end
    return other
end

local function process_update(update_kvs)
    --[[ update更新处理 ]]
    if not (type(update_kvs) == 'table' and next(update_kvs)) then return nil end
    local set = {}
    for k, v in pairs(update_kvs) do
        if type(v) == 'string' then                             -- 可接受string
            v = (v ~= '') and quote_sql_str(v) or "''"

        elseif type(v) == 'boolean' then                        -- 可接受boolean
            v = (v) and 'true' or 'false'

        elseif type(v) == 'number' then                         -- 可接受int32
            v = tonumber(v)
        end
        set[#set+ 1] = k .. '=' .. v
    end
    return concat(set, ',')
end

local gen_sql = {
    ['INSERT'] = function(table, key_value_map)
        if not (type(key_value_map) == 'table' and next(key_value_map)) then return nil end
        local keys, values = {}, {}
        for k, v in pairs(key_value_map) do
            keys[#keys + 1] = k
            if type(v) == 'string' then                         -- value 可接受string
                v = (v ~= '') and quote_sql_str(v) or "''"

            elseif type(v) == 'boolean' then                    -- value 可接受boolean
                v = v and 'true' or 'false'

            elseif type(v) == 'number' then                     -- value 可接受int32
                v = tonumber(v)
            end
            values[#values + 1] = v
        end
        return concat{'INSERT INTO ', table, '(', concat(keys, ','), ')', ' VALUES ',
            '(', concat(values, ','), ')', ';'}
    end,

    ['DELETE'] = function(table, where_params)
        if not (type(where_params) == 'table' and next(where_params)) then return nil end
        local where = process_where(where_params)
        return concat{'DELETE FROM ', table, ' WHERE ', where, ';'}
    end,

    ['UPDATE'] = function(table, update_kvs, where_params)
        if not (type(update_kvs) == 'table' and next(update_kvs)) then return nil end
        if not (type(where_params) == 'table' and next(where_params)) then return nil end
        local update = process_update(update_kvs)
        local where = process_where(where_params)
        return concat{'UPDATE ', table, ' SET ', update, ' WHERE ', where, ';'}
    end,

    ['SELECT'] = function(table, query_fields, where_params, other_params)
        local query = '*'
        if type(query_fields) == 'table' and next(query_fields) then
            query = concat(query_fields, ',')
        end
        local where = process_where(where_params)
        local other = process_other(other_params)
        return concat{'SELECT ', query, ' FROM ', table, ' WHERE ', where, other, ';'}
    end,
}

local _query = function(method, table, param1, param2, param3)
    local sql = gen_sql[method](table, param1, param2, param3)
    if not sql then
        ngx.log(ngx.ERR, 'generate sql statement error.')
        return nil
    end
    local res, err = mysql:new():query(sql)
    if not res then return nil end
    return res
end

local _muti_query = function(params)
    if not (type(params) == 'table' and next(params)) then return nil end
    local sqls = {}
    for _, param in pairs(params) do
        local sql = gen_sql['SELECT'](param[1], param[2], param[3], param[4])
        if not sql then
            ngx.log(ngx.ERR, 'generate sql statement error.')
            return nil
        end
        sqls[#sqls + 1] = sql
    end
    local res, err = mysql:new():muti_query(concat(sqls))
    if not res then return nil end
    return res
end

-- 对外接口
local insert = function(table, key_value_map)
    return _query('INSERT', table, key_value_map)
end
local delete = function(table, where_params)
    return _query('DELETE', table, where_params)
end
local update = function(table, update_kvs, where_params)
    return _query('UPDATE', table, update_kvs, where_params)
end
local query  = function(table, query_fields, where_params, other_params)
    return _query('SELECT', table, query_fields, where_params, other_params)
end
local muti_query = function(params)
    return _muti_query(params)
end

return {
    insert     = insert,
    delete     = delete,
    update     = update,
    query      = query,
    muti_query = muti_query,
}
--==================================================================================================
-- END: MYSQL 语句生成器(openresty(ngx+lua)中使用)
--==================================================================================================
