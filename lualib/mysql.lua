local mysql_c = require "resty.mysql"


--==================================================================================================
--参考资料：https://www.gitbook.com/book/moonbingbing/openresty-best-practices/details
--==================================================================================================
local _M = {}

local config = {
    ['host']     = 'xxx',
    ['port']     = 'xxx',
    ['user']     = 'xxx',
    ['password'] = 'xxx',
    ['database'] = 'xxx',
}

local commands = {
    "query",
    "muti_query",   -- customize
    "send_query",
    "read_result"
}

local mt = {__index = _M}

function _M.connect_mod(self, mysql)
    mysql:set_timeout(self.timeout)
    return mysql:connect{
        host     = config.host,
        port     = config.port,
        user     = config.user,
        password = config.password,
        database = config.database,
    }
end

function _M.set_keepalive_mod(self, mysql)
    return mysql:set_keepalive(180000, 100)
end

local function do_command(self, cmd, ...)
    local mysql, err = mysql_c:new()
    if not mysql then
        return nil, err
    end

    local ok, err, errno, sqlstate = self:connect_mod(mysql)
    if not ok then
        ngx.log(ngx.ERR, "failed to connect: ", tostring(err), ": ", tostring(errno), ": ", tostring(sqlstate), "[", cjson.encode(...), "]")
        return nil, err, errno, sqlstate
    end

    local fun = mysql[cmd]
    if cmd == 'muti_query' then -- not raw muti_query
        fun = mysql['query']
    end
    local res, err, errno, sqlstate = fun(mysql, ...)
    if not res then
        ngx.log(ngx.ERR, "bad result: ", tostring(err), ": ", tostring(errno), ": ", tostring(sqlstate), "[", cjson.encode(...), "]")
        return nil, err, errno, sqlstate
    end

    if cmd == 'muti_query' then
        local muti_res = {}
        muti_res[#muti_res + 1] = res
        local i = 2
        while err == "again" do
            res, err, errno, sqlstat = mysql['read_result'](mysql)
            if not res then
                ngx.log(ngx.ERR, "bad result #", i, ": ", tostring(err), ": ", tostring(errno), ": ", tostring(sqlstate), "[", cjson.encode(...), "]")
                return nil, err, errno, sqlstat
            end
            muti_res[#muti_res + 1] = res
        end
        self:set_keepalive_mod(mysql)
        return muti_res, err, errno, sqlstate
    end

    self:set_keepalive_mod(mysql)
    return res, err, errno, sqlstate
end

function _M.new(self, opts)
    opts = opts or {}
    local timeout = (opts.timeout and opts.timeout * 1000) or 5000

    for i = 1, #commands do
        local cmd = commands[i]
        _M[cmd] = function(self, ...)
            return do_command(self, cmd, ...)
        end
    end

    return setmetatable({timeout = timeout}, mt)
end

return _M
