local len      = string.len
local type     = type
local next     = next
local pcall    = pcall
local tonumber = tonumber
local decode   = cjson.decode
local gmatch   = ngx.re.gmatch
local insert   = table.insert

--==================================================================================================
-- AUTH : yuanfanbin
-- BEGIN: 实用工具(openresty(ngx+lua)中使用)
--==================================================================================================
--[[ 字符串split ]]
local split = function(str, p, callback)
    local rt = {}
    ngx.re.gsub(str, '[^' .. p .. ']+',
        function(w)
            local w_ = callback and callback(w[0]) or w[0]
            rt[#rt+1] = w_
            return w_
        end,
        'o')
    return rt
end

--[[ 字符串ltrim ]]
local ltrim = function(str, char)
    char = char or ' '
    if len(char) ~= 1 then
        return false, "<char> not a character, it's a string: " .. char
    end
    local newstr, n, err = ngx.re.gsub(str, "^[%"..char.."]{1,}", '', 'o')
    if newstr then
        return true, newstr
    end
    return false, err
end

--[[ 字符串rtrim ]]
local rtrim = function(str, char)
    char = char or ' '
    if len(char) ~= 1 then
        return false, "<char> not a character, it's a string: " .. char
    end
    local newstr, n, err = ngx.re.gsub(str, "[%"..char.."]{1,}$", '', 'o')
    if newstr then
        return true, newstr
    end
    return false, err
end

--[[ Lua表foreach ]]
local foreach = function(table, func)
    if type(table) ~= 'table' then
        return table
    end
    for _, value in pairs(table) do
        func(value)
    end
    return table
end

--[[ 安全模式下decode json数据 ]]
local safe_json_decode = function(data)
    local status, decode_data = pcall(decode, data)
    if not status then
        ngx.log(ngx.ERR, "cjson.decode error, data is not json. [JSON]: " ..
                tostring(data))
        return nil
    end
    return decode_data
end

--[[ 判空函数 ]]
local empty = function(data)
    if type(data) == 'table' then return not next(data) end
    if type(data) == 'string' then return data == '' end
    if type(data) == 'userdata' then return true end
    return not data
end

--[[ 版本比较 ]]
local version_compare = function(v1, v2, op)
    --[[ 仅支持[数字,小数点,空格]比较 依赖本文件的<split> ]]
    assert(v1 and v2)

    t_v1 = split(v1, '.', tonumber)
    t_v2 = split(v2, '.', tonumber)
    local cmp = {
        ['>'] = function(a, b) return a > b end,
        ['<'] = function(a, b) return a < b end,
        ['='] = function(a, b) return a == b end
    }
    if #t_v1 < #t_v2 then
        for i = #t_v1+1, #t_v2 do insert(t_v1,0) end
    elseif #t_v1 > #t_v2 then
        for i = #t_v2+1, #t_v1 do insert(t_v2,0) end
    end

    for i, v in ipairs(t_v1) do
        if not cmp['='](v, t_v2[i]) then
            return cmp[op](v, t_v2[i])
        end
    end
    if op == '=' then
        return true
    end
    return false
end

--[[ 只读表 read only table, 不可使用for pairs or for ipairs访问 ]]
local read_only = function(t)
    local _t = {}
    local mt = {
        __index = t,            -- 访问
        __newindex = function() -- 修改
            assert(false, "[[[This's read only table, please don't modify]]]")
        end
    }
    setmetatable(_t, mt)
    return _t;
end

--[[ 深拷贝 clone - ref 'coco2dx' ]]
local clone = function(object)
    local lookup_table = {}
    local function _copy(object)
        if type(object) ~= 'table' then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for key, value in pairs(object) do
            new_table[_copy(key)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return _copy(object)
end

--[[ 多表合并，(不保留key) ]]
local merge = function(...)
    local hash = {}
    local tables = {...}
    for _, table in pairs(tables) do
        for _, value in pairs(table) do
            insert(hash, value)
        end
    end
    return hash
end

--[[ 多表合并，保留key/value(注：key若多表相同会被覆盖，保留最后一个key/value) ]]
local merge_kv = function(...)
    local hash = {}
    local tables = {...}
    for _, table in pairs(tables) do
        for k, v in pairs(table) do
            hash[k] = v
        end
    end
    return hash
end

return {
    split            = split,
    ltrim            = ltrim,
    rtrim            = rtrim,
    empty            = empty,
    clone            = clone,
    merge            = merge,
    merge_kv         = merge_kv,
    foreach          = foreach,
    read_only        = read_only,
    version_compare  = version_compare,
    safe_json_decode = safe_json_decode,
}
--==================================================================================================
-- END: 实用工具(openresty(ngx+lua)中使用)
--==================================================================================================
