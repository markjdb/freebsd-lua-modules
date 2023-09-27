/*
 * Copyright (c) Mark Johnston <markj@FreeBSD.org>
 */

#include <sys/utsname.h>

#include <errno.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int	luaopen_uname(lua_State *L);

static int
l_utsname(lua_State *L)
{
	struct utsname name;
	int error;

	if (uname(&name) != 0) {
		error = errno;
		snprintf(errmsg, sizeof(errmsg), "uname: %s", strerror(errno));
		lua_pushnil(L);
		lua_pushstring(L, strerror(error));
		return (2);
	}

	lua_newtable(L);
#define	setkv(f) do {			\
	lua_pushstring(L, name.f);	\
	lua_setfield(L, -2, #f);	\
} while (0)
	setkv(sysname);
	setkv(nodename);
	setkv(release);
	setkv(version);
	setkv(machine);
#undef setkv

	return (1);
}

/*
 * XXX-MJ it'd make more sense if the module was a single function instead of a
 * table...
 */
static const struct luaL_Reg l_uname[] = {
	{"utsname", l_utsname},
	{NULL, NULL},
};

int
luaopen_uname(lua_State *L)
{
	lua_newtable(L);

	luaL_setfuncs(L, l_uname, 0);

	return (1);
}
