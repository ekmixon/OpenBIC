/*
 * Copyright (c) 2020 rxi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "log.h"

static struct {
    void *udata;
    log_LockFn lock;
    int level;
    bool quiet;
} L;

static const char *level_strings[] = { "TRACE", "DEBUG", "INFO",
				       "WARN",	"ERROR", "FATAL" };

#if CONFIG_LOG_COLOR
static const char *level_colors[] = { "\x1b[94m", "\x1b[36m", "\x1b[32m",
				      "\x1b[33m", "\x1b[31m", "\x1b[35m" };
#endif

static void stdout_callback(log_Event *ev)
{
#if CONFIG_LOG_COLOR
    printf("%s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ", level_colors[ev->level],
	   level_strings[ev->level], ev->file, ev->line);
#else
    printf("%-5s %s:%d: ", level_strings[ev->level], ev->file, ev->line);
#endif
    vprintf(ev->fmt, ev->ap);
}

static void lock(void)
{
    if (L.lock) {
	L.lock(true, L.udata);
    }
}

static void unlock(void)
{
    if (L.lock) {
	L.lock(false, L.udata);
    }
}

const char *log_level_string(int level)
{
    return level_strings[level];
}

void log_set_lock(log_LockFn fn, void *udata)
{
    L.lock = fn;
    L.udata = udata;
}

void log_set_level(int level)
{
    L.level = level;
}

void log_set_quiet(bool enable)
{
    L.quiet = enable;
}

void log_init(void)
{
    memset(&L, 0, sizeof(L));
    log_set_level(CONFIG_LOG_LEVEL);
}

void log_log(int level, const char *file, int line, const char *fmt, ...)
{
    log_Event ev = {
	.fmt = fmt,
	.file = file,
	.line = line,
	.level = level,
    };

    lock();

    if (!L.quiet && level >= L.level) {
	va_start(ev.ap, fmt);
	stdout_callback(&ev);
	va_end(ev.ap);
    }

    unlock();
}