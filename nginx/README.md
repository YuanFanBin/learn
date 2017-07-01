nginx 1.8.0 源码学习-基础知识
=============================

tags: nginx

----
目录
====
* [0. 待办事宜](#0-待办事宜)
* [1. nginx源码目录结构](#1-nginx源码目录结构)
* [2. nginx中基本结构体](#2-nginx中基本结构体)
    * [2.1 ngx_module_t](#21-ngxmodulet "模块上下文结构")
        * [2.1.1 ngx_http_module_t](#211-ngxhttpmodulet "HTTP模块结构")
        * [2.1.2 ngx_core_module_t](#212-ngxcoremodulet "CORE模块结构")
        * [2.1.3 ngx_event_module_t](#213-ngxeventmodulet "EVENT模块结构")
        * [2.1.4 ngx_mail_module_t](#214-ngxmailmodulet "MAIL模块结构")
    * [2.2 ngx_conf_t](#22-ngxconft "模块配置文件结构")
    * [2.7 ngx_open_file_t](#27-ngxopenfilet "打开文件模块结构")
    * [2.8 ngx_command_t](#28-ngxcommandt "模块指令数据结构")
* [3. nginx中基本数据结构](#3-nginx中基本数据结构)
* [4. nginx中的HTTP大模块](#4-nginx中的http大模块)
    * [4.1 http模块中的基本结构](#41-http模块中的基本结构)
        * [4.1.1 ngx_http_core_main_conf_t](#411-ngxhttpcoremainconft)
        * [4.1.2 ngx_http_core_srv_conf_t](#412-ngxhttpcoresrvconft)
        * [4.1.3 ngx_http_core_loc_conf_t](#413-ngxhttpcorelocconft)
* [参考资料](#参考资料)
* [声明](#声明)

-----------
0. 待办事宜
===========
- [x] 目录基本结构
- [ ] 学习基本struct结构
- [x] 2.1 ngx_command_t
- [x] 2.2 ngx_module_t
- [x] 2.3 ngx_conf_t
- [ ] 3.1 ngx_str_t
- [ ] 学习资料，了解如何写扩展
- [ ] 完成自己的hello扩展

[Back to TOC](#目录)

--------------------
1. nginx源码目录结构
====================
|目录|说明|
|---|---|
|./auto|自动检测系统环境以及编译相关的脚本|
|./auto/cc|关于编译器相关的编译选项的检测脚本|
|./auto/lib|nginx编译所需要的一些库的检测脚本|
|./auto/os|与平台相关的一些系统参数与系统调用相关的检测|
|./auto/types|与数据类型相关的一些辅助脚本|
|./conf|存放默认配置文件，在`make install`后，会拷贝到安装目录中去|
|./contrib|存放一些实用工具|
|./html|存放默认的网页文件，在`make install`后，会拷贝到安装目录中去|
|./man|nginx的man手册|
|./objs|nginx的.o文件及 `ngx_auto_config.h`, `ngx_auto_headers.h`, `ngx_modules.c`|
|./src|存放nginx源码文件|
|./src/core|nginx的核心源代码，包括常用数据结构的定义及nginx初始化运行的核心代码，如`main`函数|
|./src/event|对系统事件处理机制的封装，以及定时器的实现相关代码|
|./src/event/modules|不同事件处理方式的模块化，如select, poll, epoll, kqueue等|
|./src/http|nginx作为http服务器相关的代码|
|./src/http/modules|包含http的各种功能模块|
|./src/mail|nginx作为邮件代理服务器相关的代码|
|./src/misc|一些辅助代码，测试c++头的兼容性，以及对google_perftools的支持|
|./src/os|主要是对各种不同体系结构所提供的系统函数的封装，对外提供统一的系统调用接口|

随后学习内容主要涉及以下四个目录`./src/core`, `./src/event`, `./src/http`, `./src/os`

**本小节参考资料**

http://tengine.taobao.org/book/chapter_09.html#nginx-100

[Back to TOC](#目录)

--------------------
2. nginx中基本结构体
====================

代码文件`./src/core/ngx_core.h`

```c
#include <ngx_config.h>

typedef struct ngx_module_s      ngx_module_t;
typedef struct ngx_conf_s        ngx_conf_t;
typedef struct ngx_cycle_s       ngx_cycle_t;
typedef struct ngx_pool_s        ngx_pool_t;
typedef struct ngx_chain_s       ngx_chain_t;
typedef struct ngx_log_s         ngx_log_t;
typedef struct ngx_open_file_s   ngx_open_file_t;
typedef struct ngx_command_s     ngx_command_t;
typedef struct ngx_file_s        ngx_file_t;
typedef struct ngx_event_s       ngx_event_t;
typedef struct ngx_event_aio_s   ngx_event_aio_t;
typedef struct ngx_connection_s  ngx_connection_t;

#if (NGX_THREADS)
typedef struct ngx_thread_task_s  ngx_thread_task_t;
#endif
```

[Back to TOC](#目录)

------------
2.1 ngx_module_t
------------

声明：`./src/core/ngx_core.h`

定义：`./src/core/ngx_conf_file.h`

用途：模块上下文结构，定义模块基本结构，注册自定义模块的重要结构

```c
#define NGX_MODULE_V1          0, 0, 0, 0, 0, 0, 1
#define NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0 

struct ngx_module_s {
    ngx_uint_t            ctx_index;
    ngx_uint_t            index;
    
    ngx_uint_t            spare0;
    ngx_uint_t            spare1;
    ngx_uint_t            spare2;
    ngx_uint_t            spare3;

    ngx_uint_t            version;

    void                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *log);

    ngx_int_t           (*init_module)(ngx_cycle_t *cycle);

    ngx_int_t           (*init_process)(ngx_cycle_t *cycle);
    ngx_int_t           (*init_thread)(ngx_cycle_t *cycle);
    void                (*exit_thread)(ngx_cycle_t *cycle);
    void                (*exit_process)(ngx_cycle_t *cycle);

    void                (*exit_master)(ngx_cycle_t *cycle);

    uintptr_t             spare_hook0;
    uintptr_t             spare_hook1;
    uintptr_t             spare_hook2;
    uintptr_t             spare_hook3;
    uintptr_t             spare_hook4;
    uintptr_t             spare_hook5;
    uintptr_t             spare_hook6;
    uintptr_t             spare_hook7;
};
```

**结构中前7个参数及后8个参数，我们通常不用设置**，使用宏定义 `NGX_MODULE_V1`,  `NGX_MODULE_V1_PADDING` 即可，其中带有 `spare` 的参数暂未使用； 

`index`：文件 `./objs/ngx_modules.c` 在执行`$ ./configure` 时由 `./auto/make` 自动生成，生成的`*ngx_modules[]`结构对应的模块索引对应结构中的`index`字段，赋值操作发生在 *main* ( `./src/core/nginx.c` )函数的如下代码中

```c
ngx_max_module = 0;
for (i = 0; ngx_modules[i]; i++) {
    ngx_modules[i]->index = ngx_max_module++;
}
```

`ctx_index`：在文件 `./src/http/ngx_http.c` 中的 *ngx_http_block* 函数中定义，如下代码仅截取了 `http` 模块的代码片段，在文件 `./src/event/ngx_event.c` , `./src/mail/ngx_mail.c` 中有类似定义，即`ctx_index`为在同种模块中子模块的索引

```c
/* count the number of the http modules and set up their indices */

ngx_http_max_module = 0;                                                         
for (m = 0; ngx_modules[m]; m++) {                                               
    if (ngx_modules[m]->type != NGX_HTTP_MODULE) {                               
        continue;                                                                
    }                                                                            
    ngx_modules[m]->ctx_index = ngx_http_max_module++;
} 
```

`version`：模块版本

`ctx`：模块相关的上下文环境，在这里定义为 `void *` 是为了方便每个模块定义自定的`ctx`时使用，对于 `http` 模块来说 [ngx_http_module_t](#211-ngxhttpmodulet) 定义了该结构

`commands`：命令结构组，由 [ngx_command_t](#28-ngxcommandt) 定义，表示一组配置文件中可用的配置项（指令）

`type`：模块类型，用于区分 `core` , `event` , `http` , `mail`，由宏 *NGX_[CORE|EVENT|HTTP|MAIL]_MODULE* 定义

`init_[master|module|process|thread]`：在各自相应初始化阶段调用init函数

`exit_[thread|process|master]`：在各自退出阶段调用exit函数

**本小结参考资料**
http://tengine.taobao.org/book/chapter_03.html#id5
http://segmentfault.com/a/1190000002778510
http://my.oschina.net/showcolors/blog/101169

[Back to TOC](#目录)

-----------------------
2.1.1 ngx_http_module_t
-----------------------

声明：`./src/http/ngx_http_config.h`

定义：`./src/http/ngx_http_config.h`

用途：HTTP模块结构，用于定义http模块的子模块结构，提供了一组回调函数指针，该结构在加载和处理配置文件时，调用相应回调结构处理配置信息。（与 `http` 平级的还有 `event` , `mail` , `core` 等）

```c
typedef struct {
    ngx_int_t   (*preconfiguration)(ngx_conf_t *cf);
    ngx_int_t   (*postconfiguration)(ngx_conf_t *cf);

    void       *(*create_main_conf)(ngx_conf_t *cf);
    char       *(*init_main_conf)(ngx_conf_t *cf, void *conf);

    void       *(*create_srv_conf)(ngx_conf_t *cf);
    char       *(*merge_srv_conf)(ngx_conf_t *cf, void *prev, void *conf);

    void       *(*create_loc_conf)(ngx_conf_t *cf);
    char       *(*merge_loc_conf)(ngx_conf_t *cf, void *prev, void *conf);
} ngx_http_module_t;
```

`preconfiguration`：在创建和读取指定模块的配置信息前调用，通常会设置一些全局变量 *variable*

`postconfiguration`：在创建和读取指定模块的配置信息后调用，通常会设置一些 *handler* 和 *filter* 

`create_[main|srv|loc]_conf`：配置指令的 `type` 字段用宏定义 *NGX_HTTP_[MAIN|SRV|LOC]_CONF* 指定了命令的作用域范围。

`merge_[main|srv|loc]_conf`：用于合并配置信息，摘录 [Tengine](http://tengine.taobao.org/book/chapter_03.html#id4) 对合并的描述

> nginx里面的配置信息都是上下一层层嵌套的，对于具体某个location的话，对于同一个配置，如果当前层次没有定义那么就使用上层的配置，否则使用当前层次的配置。

**之后学习的主要方向也是编写HTTP相关的子模块的扩展**

**本小结参考资料**
http://tengine.taobao.org/book/chapter_03.html#id4

[Back to TOC](#目录)

-----------------------
2.1.2 ngx_core_module_t
-----------------------

声明：`./src/core/ngx_conf_file.h`

定义：`./src/http/ngx_conf_file.h`

用途：CORE模块结构，用于定义CORE模块的子模块结构，提供创建及初始化的函数指针

```c
typedef struct {
    ngx_str_t             name;
    void               *(*create_conf)(ngx_cycle_t *cycle);
    char               *(*init_conf)(ngx_cycle_t *cycle, void *conf);
} ngx_core_module_t;
```

`name`：nginx核心模块名称

`create_conf`：创建核心模块结构函数指针

`init_conf`：读取配置文件后，利用此函数指针初始化相关的核心模块

本版本的nginx中，利用 `grep -nR --color "static ngx_core_module_t" .` 可以快捷列举出当前版本的核心模块，如：`core`, `thread_pool`, `errlog`, `regex`, `http`, `events`, `openssl`, `mail`, `google_perftools`


[Back to TOC](#目录)

------------------------
2.1.3 ngx_event_module_t
------------------------

声明：`./src/event/ngx_event.h`

定义：`./src/event/ngx_event.h`

用途：EVENT模块结构，用于定义EVENT模块的子模块结构，提供创建及初始化的函数指针，以及actions

```c
typedef struct {
    ngx_str_t              *name;

    void                 *(*create_conf)(ngx_cycle_t *cycle);
    char                 *(*init_conf)(ngx_cycle_t *cycle, void *conf);

    ngx_event_actions_t     actions;                                                
} ngx_event_module_t;
```

[Back to TOC](#目录)

-----------------------
2.1.4 ngx_mail_module_t
-----------------------

[Back to TOC](#目录)

--------------
2.2 ngx_conf_t
--------------

声明：`./src/core/ngx_core.h`

定义：`./src/core/ngx_conf_file.h`

用途：保存解析配置文件时所留存的名称，参数，上下文环境等信息，很重要的一个数据结构，很多内部函数都会有此结构的参数。

```c
struct ngx_conf_s {
    char                 *name;
    ngx_array_t          *args;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *temp_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    void                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    char                 *handler_conf;
};
```

`name`：当前解析到的命令名称

`args`：当前命令所拥有的参数

`cycle`： 使用的cycle

`pool`：解析此命令时所使用的内存池

`temp_pool`：解析配置文件所用的临时内存池

`conf_file`：将要解析的配置文件

`log`： log日志

`ctx`：当前解析的上下文环境

`module_type`：模块类型，同 [ngx_module_t](#21-ngxmodulet) 的 `type`

`cmd_type`：命令类型, NGX_[MAIN|SVR|LOC]_CONF

`handler`：指令自定义的handler（费了半天劲用GDB跟踪出来了，后面讲解跟踪过程）

`handler_conf`：自定义handler的conf，块不同(例：http, server, location)相应指令参数可能不同，会有merge

以上内容的详细解析需要查看源码来理解，我们先来看 `./src/core/nginx.c` 的 *main* 函数对`core`模块的 *conf* 的初始化， **即整个nginx的入口处** ；

```c
cycle = ngx_init_cycle(&init_cycle);
if (cycle == NULL) {
    if (ngx_test_config) {
        ngx_log_stderr(0, "configuration file %s test failed",
                       init_cycle.conf_file.data);
    }

    return 1;
}
```

函数 *ngx_init_cycle* 为初始化 *conf* 的入口，跟踪进入函数内，会找到如下代码( `./src/core/ngx_cycle.c` )

```c
for (i = 0; ngx_modules[i]; i++) {
    /* 优先加载CORE模块 */
    if (ngx_modules[i]->type != NGX_CORE_MODULE) {
        continue;
    }

    module = ngx_modules[i]->ctx;

    if (module->create_conf) {
        rv = module->create_conf(cycle);
        if (rv == NULL) {
            ngx_destroy_pool(pool);
            return NULL;
        }
        /* cycle的conf_ctx存放了对应模块的main conf */
        cycle->conf_ctx[ngx_modules[i]->index] = rv;
    }
}
```

如上代码创建了 `NGX_CORE_MODULE` 模块的配置文件结构( [ngx_core_module_t](#212-ngxcoremodulet) )的信息，实际的初始化操作还未进行；再看一段代码

```c
    ngx_memzero(&conf, sizeof(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, sizeof(ngx_str_t));
    if (conf.args == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }

    conf.temp_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    if (conf.temp_pool == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }


    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#if 0
    log->log_level = NGX_LOG_DEBUG_ALL;
#endif

    if (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        return NULL;
    }

    if (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        return NULL;
    }

    if (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.data);
    }
```

如上代码初始化了 `CORE模块` 的整个 *conf* 结构； 其中核心函数为 *ngx_conf_parse* （函数可以用于解析文件，块结构，或者指令参数），在该函数中，我们主要关心如下代码

```c
/* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

if (cf->handler) {

    /*
     * the custom handler, i.e., that is used in the http's
     * "types { ... }" directive
     */

    if (rc == NGX_CONF_BLOCK_START) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
        goto failed;
    }

    rv = (*cf->handler)(cf, NULL, cf->handler_conf);
    if (rv == NGX_CONF_OK) {
        continue;
    }

    if (rv == NGX_CONF_ERROR) {
        goto failed;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, rv);

    goto failed;
}

rc = ngx_conf_handler(cf, rc);

if (rc == NGX_ERROR) {
    goto failed;
}
```

**其中我们需要再次深入理解的两行代码为 `rv = (*cf->handler)(cf, NULL, cf->handler_conf);`, `rc = ngx_conf_handler(cf, rc);`， 当对应的conf有handler时，调用前者处理否则调用后者的默认指令处理方式。**

**我们用gdb跟踪先来看看前者的handler究竟是什么**

```gdb
$ sudo cgdb ./obj/nginx
(gdb) b src/core/ngx_conf_file.c:232    # 即 `rv = (*cf->handler)(cf, NULL, cf->handler_conf);`
(gdb) r
(gdb) p cf->handler                     # 查看当前的handler
$1 = (ngx_conf_handler_pt) 0x806cc79 <ngx_http_core_type> # OK! 即将调用的函数明了
(gdb) s                                 # 进入handler对应函数ngx_http_core_type
```

由cgdb（gdb也是一样的用法）调试出 `handler` 某一个调用函数实际为 *ngx_http_core_type*  ，我们在搜索一下此函数在何处使用，实际搜索结果显示除了声明及定义除外，仅有如下函数会直接使用此函数名称

```c
static char *
ngx_http_core_types(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    char        *rv;
    ngx_conf_t   save;

    if (clcf->types == NULL) {
        clcf->types = ngx_array_create(cf->pool, 64, sizeof(ngx_hash_key_t));
        if (clcf->types == NULL) {
            return NGX_CONF_ERROR;
        }
    }

    save = *cf;
    cf->handler = ngx_http_core_type; /* here */
    cf->handler_conf = conf;

    rv = ngx_conf_parse(cf, NULL);

    *cf = save;

    return rv;
}
```

再跟踪函数我们可定位到函数的定义及相应的指令 **types** ( `./src/http/ngx_http_core_module.c` )，最终我们会跟踪到此处（需要注意如下参数 `NGX_CONF_BLOCK`，此参数指代了此指令是一个块结构）

```c
static ngx_command_t  ngx_http_core_commands[] = {
    ...
    { ngx_string("types"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF
                                          |NGX_CONF_BLOCK|NGX_CONF_NOARGS,
      ngx_http_core_types,  /* here */
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },
    ...
}
```

也就是说此 `handler` 为函数 `ngx_http_core_type`，我们再仔细看一下源码，

```c
    save = *cf;
    cf->handler = ngx_http_core_type;
    cf->handler_conf = conf;
    rv = ngx_conf_parse(cf, NULL);
    *cf = save;
```

其中 `save = *cf`, `*cf = save` 只是保存、恢复调用 `ngx_conf_parse` 前后的上下文环境，设置handler和handler_conf后，又调用 *ngx_conf_parse* 去解析配置，注意此时第二个参数为NULL，gdb进入后会执行 `type = parse_block` 而不是 `type = parse_file`。

以上内容主要是代码上的分析，现在我们来看一看命令 **types** 在nginx中的配置，由如下配置可看出命令 **types** 是一个块结构，`ngx_conf_parse` 调用时需解析此块内容

```nginx.conf
...
http {
    include       mime.types;
    ...
}
```

```mime.types
# mime.types
types {
    text/html                             html htm shtml;
    text/css                              css;
    text/xml                              xml;
    image/gif                             gif;
    ...
}
```

综上， `rv = (*cf->handler)(cf, NULL, cf->handler_conf);` 所对应的 `handler` 为某个块指令对应的块处理函数 **（并不是所有块指令都拥有此handler）** 。


**现在我们来进入 `rc = ngx_conf_handler(cf, rc);` 来分析默认的handler的处理流程**

我们直接进入函数 `ngx_conf_handler`中看核心代码块

```c
/* set up the directive's configuration context */

conf = NULL;

if (cmd->type & NGX_DIRECT_CONF) {
    conf = ((void **) cf->ctx)[ngx_modules[i]->index];

} else if (cmd->type & NGX_MAIN_CONF) {
    conf = &(((void **) cf->ctx)[ngx_modules[i]->index]);

} else if (cf->ctx) {
    confp = *(void **) ((char *) cf->ctx + cmd->conf);

    if (confp) {
        conf = confp[ngx_modules[i]->ctx_index];
    }
}

rv = cmd->set(cf, cmd, conf); 
```

如上代码，抛开conf赋值的逻辑，可以理解为调用指令对应的set操作指令赋值(参考 [ngx_command_t](#28-ngxcommandt) )

如上内容解释了 `handler`, `handler_conf`， **`ctx` 的含义我们还需继续分析源码**，有了之前的调试基础，我们在 `rc = ngx_conf_handler(cf, rc);` 函数内的 `rv = cmd->set(cf, cmd, conf);` 设置断点并进入函数内分析

```gdb
$ sudo cgdb ./obj/nginx
(gdb) b src/core/ngx_conf_file.c:391 if strncmp(name->data, "server", 6) == 0
(gdb) p *cmd
$1 = {name = {len = 6, data = 0x80b54df "server"}, type = 33554689, set = 0x806f81a <ngx_http_core_server>, conf = 0, offset = 0, post = 0x0}
(gdb) s
```

```c
/* 仅截取几个本小结关注的关键代码 */
static char *                                                                    
ngx_http_core_server(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy)
{
    ...
    ctx->main_conf = http_ctx->main_conf;
    for (i = 0; ngx_modules[i]; i++) {
        if (ngx_modules[i]->type != NGX_HTTP_MODULE) {
            continue;
        }
        module = ngx_modules[i]->ctx;
        if (module->create_srv_conf) {
            mconf = module->create_srv_conf(cf);
            ctx->srv_conf[ngx_modules[i]->ctx_index] = mconf;
        }
        if (module->create_loc_conf) {
            mconf = module->create_loc_conf(cf);
            ctx->loc_conf[ngx_modules[i]->ctx_index] = mconf;
        }
    }
    /* the server configuration context */
    cscf = ctx->srv_conf[ngx_http_core_module.ctx_index];
    cscf->ctx = ctx;
    cmcf = ctx->main_conf[ngx_http_core_module.ctx_index];
    cscfp = ngx_array_push(&cmcf->servers);
    if (cscfp == NULL) {
        return NGX_CONF_ERROR;
    }
    *cscfp = cscf;
    
    /* parse inside server{} */
    pcf = *cf;
    cf->ctx = ctx;
    cf->cmd_type = NGX_HTTP_SRV_CONF;
    rv = ngx_conf_parse(cf, NULL);
    *cf = pcf;
    ...
}
```

以上代码很容易理解，此时的 `ctx` 指代的就是当前的 `server` 指令对应的上下文环境，并将此 `cscf` 结构存储于http结构的servers中，随后调用 `ngx_conf_parse` 解析块指令 `server {...}`，当然此时可能出现冲突，会由 `merge_srv_conf` 解决（详细分析不在本小结做）

**本小结参考资料**
http://www.pagefault.info/?p=368
http://tengine.taobao.org/book/chapter_11.html#id4

[Back to TOC](#目录)

-------------------
2.7 ngx_open_file_t
-------------------

声明：`./src/core/ngx_core.h`

定义：`./src/core/ngx_conf_file.c`

用途：用于打开文件

```c
struct ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;
    void                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    void                 *data;
};
```

`fd`：打开的文件描述符；

`name`：打开的文件名称；

`flush`：

`data`：

[Back to TOC](#目录)

-----------------
2.8 ngx_command_t
-----------------

声明：`./src/core/ngx_core.h`

定义：`./src/core/ngx_conf_file.h`

用途：模块指令数据结构，用于定义nginx配置文件命令

```c
struct ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    void                 *post;
};

#define ngx_null_command  { ngx_null_string, 0, NULL, 0, 0, NULL }
```

`name`：命令类型，配置文件中的配置的参数名称，例`limit_conn_zone`，使用 *ngx_str_t* 结构来扩展自定义名称

`type`：命令的作用范围( *main*, *server*, *locatin* )及可接受的参数个数，作用范围：`./src/http/ngx_http_config.h`, 可接受参数个数：`./src/core/ngx_conf_file.h`

`set`：执行命令的函数，当nginx在解析配置的时候，遇到配置指令 `name`，将会把读取到的内容传递给此函数处理（存储/初始化等），nginx提供了一些常用的set函数 *ngx_conf_set_xxx_xxx* (`./src/core/ngx_conf_file.c`)；何处执行了 `set` 操作，参照 [ngx_conf_t](#22-ngxconft) 的 `handler` 

`conf`：HTTP模块中配置信息 *main_conf*, *serv_conf*, *loc_conf* 在 `ngx_http_conf_ctx_t` 中的偏移范围指定具体配置文件，偏移量由系统宏 *offsetof* 计算( *man offsetof* 查看用法)，可接受参数  *NGX_HTTP_xxx_CONF_OFFSET* ( `./src/http/ngx_http_config.h` )

`offset`：指定该配置项值的精确存放位置，一般指定为某一个结构体变量的字段偏移，使用 *offsetof* 宏

`post`：可以指向任何一个在读取配置过程中需要的数据，以便于进行配置读取的处理；通常设为NULL

**本小结参考资料**
http://tengine.taobao.org/book/chapter_03.html#id3
http://www.cnblogs.com/jzhlin/archive/2012/06/09/module.html

[Back to TOC](#目录)

----------------------
3. nginx中基本数据结构
======================

-------------
3.1 ngx_str_t
-------------

```c
typedef struct {                                                                    
    size_t      len;
    u_char     *data;
} ngx_str_t;
```

[Back to TOC](#目录)

本小结参考资料

http://tengine.taobao.org/book/chapter_02.html#id3

http://blog.csdn.net/livelylittlefish/article/category/838107

----------------------
4. nginx中的HTTP大模块
======================

## 4.1 http模块中的基本结构

### 4.1.1 ngx_http_core_main_conf_t

声明：`./src/http/ngx_http_core_module.h`

定义：`./src/http/ngx_http_core_module.h`

用途：`HTTP {}` 块解析时，存储仅作用于该块的配置。

```c
typedef struct {
    ngx_array_t                servers;         /* ngx_http_core_srv_conf_t */
    
    ngx_http_phase_engine_t    phase_engine;
    
    ngx_hash_t                 headers_in_hash;
    
    ngx_hash_t                 variables_hash;
    
    ngx_array_t                variables;       /* ngx_http_variable_t */      
    ngx_uint_t                 ncaptures;
    
    ngx_uint_t                 server_names_hash_max_size;
    ngx_uint_t                 server_names_hash_bucket_size; 
    
    ngx_uint_t                 variables_hash_max_size;
    ngx_uint_t                 variables_hash_bucket_size;
    
    ngx_hash_keys_arrays_t    *variables_keys;
    
    ngx_array_t               *ports;

    ngx_uint_t                 try_files;       /* unsigned  try_files:1 */

    ngx_http_phase_t           phases[NGX_HTTP_LOG_PHASE + 1];
} ngx_http_core_main_conf_t;
```

### 4.1.2 ngx_http_core_srv_conf_t

声明：`./src/http/ngx_http_core_module.h`

定义：`./src/http/ngx_http_core_module.h`

用途：`SERVER {}` 解析时，存储可作用于 `http, server` 的配置。

```c
typedef struct {
    /* array of the ngx_http_server_name_t, "server_name" directive */
    ngx_array_t                 server_names;
    
    /* server ctx */
    ngx_http_conf_ctx_t        *ctx;
    
    ngx_str_t                   server_name;                   
    
    size_t                      connection_pool_size;          
    size_t                      request_pool_size;             
    size_t                      client_header_buffer_size;     
    
    ngx_bufs_t                  large_client_header_buffers;   
    
    ngx_msec_t                  client_header_timeout;         
    
    ngx_flag_t                  ignore_invalid_headers;        
    ngx_flag_t                  merge_slashes;                 
    ngx_flag_t                  underscores_in_headers;        
    
    unsigned                    listen:1;
#if (NGX_PCRE)
    unsigned                    captures:1;                    
#endif

    ngx_http_core_loc_conf_t  **named_locations;
} ngx_http_core_srv_conf_t;
```

### 4.1.3 ngx_http_core_loc_conf_t

用途：`LOCATION {}` 解析时，存储可作用于 `http, server, locatin` 的配置。

```c
struct ngx_http_core_loc_conf_s {
    ngx_str_t     name;          /* location name */

#if (NGX_PCRE)
    ngx_http_regex_t  *regex;
#endif

    unsigned      noname:1;   /* "if () {}" block or limit_except */
    unsigned      lmt_excpt:1;
    unsigned      named:1;

    unsigned      exact_match:1;
    unsigned      noregex:1;

    unsigned      auto_redirect:1;
#if (NGX_HTTP_GZIP)
    unsigned      gzip_disable_msie6:2;
#if (NGX_HTTP_DEGRADATION)
    unsigned      gzip_disable_degradation:2;
#endif
#endif

    ngx_http_location_tree_node_t   *static_locations;
#if (NGX_PCRE)
    ngx_http_core_loc_conf_t       **regex_locations;
#endif

    /* pointer to the modules' loc_conf */
    void        **loc_conf;

    uint32_t      limit_except;
    void        **limit_except_loc_conf;

    ngx_http_handler_pt  handler;

    /* location name length for inclusive location with inherited alias */
    size_t        alias;
    ngx_str_t     root;                    /* root, alias */
    ngx_str_t     post_action;

    ngx_array_t  *root_lengths;
    ngx_array_t  *root_values;

    ngx_array_t  *types;
    ngx_hash_t    types_hash;
    ngx_str_t     default_type;

    off_t         client_max_body_size;    /* client_max_body_size */
    off_t         directio;                /* directio */
    off_t         directio_alignment;      /* directio_alignment */

    size_t        client_body_buffer_size; /* client_body_buffer_size */
    size_t        send_lowat;              /* send_lowat */
    size_t        postpone_output;         /* postpone_output */
    size_t        limit_rate;              /* limit_rate */
    size_t        limit_rate_after;        /* limit_rate_after */
    size_t        sendfile_max_chunk;      /* sendfile_max_chunk */
    size_t        read_ahead;              /* read_ahead */

    ngx_msec_t    client_body_timeout;     /* client_body_timeout */
    ngx_msec_t    send_timeout;            /* send_timeout */
    ngx_msec_t    keepalive_timeout;       /* keepalive_timeout */
    ngx_msec_t    lingering_time;          /* lingering_time */
    ngx_msec_t    lingering_timeout;       /* lingering_timeout */
    ngx_msec_t    resolver_timeout;        /* resolver_timeout */

    ngx_resolver_t  *resolver;             /* resolver */

    time_t        keepalive_header;        /* keepalive_timeout */

    ngx_uint_t    keepalive_requests;      /* keepalive_requests */
    ngx_uint_t    keepalive_disable;       /* keepalive_disable */
    ngx_uint_t    satisfy;                 /* satisfy */
    ngx_uint_t    lingering_close;         /* lingering_close */
    ngx_uint_t    if_modified_since;       /* if_modified_since */
    ngx_uint_t    max_ranges;              /* max_ranges */
    ngx_uint_t    client_body_in_file_only; /* client_body_in_file_only */

    ngx_flag_t    client_body_in_single_buffer;
                                           /* client_body_in_singe_buffer */
    ngx_flag_t    internal;                /* internal */
    ngx_flag_t    sendfile;                /* sendfile */
    ngx_flag_t    aio;                     /* aio */
    ngx_flag_t    tcp_nopush;              /* tcp_nopush */
    ngx_flag_t    tcp_nodelay;             /* tcp_nodelay */
    ngx_flag_t    reset_timedout_connection; /* reset_timedout_connection */
    ngx_flag_t    server_name_in_redirect; /* server_name_in_redirect */
    ngx_flag_t    port_in_redirect;        /* port_in_redirect */
    ngx_flag_t    msie_padding;            /* msie_padding */
    ngx_flag_t    msie_refresh;            /* msie_refresh */
    ngx_flag_t    log_not_found;           /* log_not_found */
    ngx_flag_t    log_subrequest;          /* log_subrequest */
    ngx_flag_t    recursive_error_pages;   /* recursive_error_pages */
    ngx_flag_t    server_tokens;           /* server_tokens */
    ngx_flag_t    chunked_transfer_encoding; /* chunked_transfer_encoding */
    ngx_flag_t    etag;                    /* etag */

#if (NGX_HTTP_GZIP)
    ngx_flag_t    gzip_vary;               /* gzip_vary */

    ngx_uint_t    gzip_http_version;       /* gzip_http_version */
    ngx_uint_t    gzip_proxied;            /* gzip_proxied */

#if (NGX_PCRE)
    ngx_array_t  *gzip_disable;            /* gzip_disable */
#endif
#endif

#if (NGX_THREADS)
    ngx_thread_pool_t         *thread_pool;
    ngx_http_complex_value_t  *thread_pool_value;
#endif

#if (NGX_HAVE_OPENAT)
    ngx_uint_t    disable_symlinks;        /* disable_symlinks */
    ngx_http_complex_value_t  *disable_symlinks_from;
#endif

    ngx_array_t  *error_pages;             /* error_page */
    ngx_http_try_file_t    *try_files;     /* try_files */

    ngx_path_t   *client_body_temp_path;   /* client_body_temp_path */

    ngx_open_file_cache_t  *open_file_cache;
    time_t        open_file_cache_valid;
    ngx_uint_t    open_file_cache_min_uses;
    ngx_flag_t    open_file_cache_errors;
    ngx_flag_t    open_file_cache_events;

    ngx_log_t    *error_log;

    ngx_uint_t    types_hash_max_size;
    ngx_uint_t    types_hash_bucket_size;

    ngx_queue_t  *locations;

#if 0
    ngx_http_core_loc_conf_t  *prev_location;
#endif
};
```

[Back to TOC](#目录)

--------
参考资料
========

http://tengine.taobao.org/book/index.html

http://blog.codinglabs.org/articles/intro-of-nginx-module-development.html

[Back to TOC](#目录)

----
声明
====

所有内容学习基于 [Github](https://github.com/ "Github") 上淘宝的开源项目 [nginx-book](https://github.com/taobao/nginx-book "nginx-book")，及 [Nginx](http://nginx.org/ "Nginx") 的开源代码 [stable 1.8.0](http://nginx.org/download/nginx-1.8.0.tar.gz) ，所有学习内容自用。

[Back to TOC](#目录)