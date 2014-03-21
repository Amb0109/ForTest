
test_cb = TestCB:new()

test_cb:print()

function lua_callback(a, b)
	print ("hello ", (a + b))
end

test_cb:set_lua_func(lua_callback)

test_cb:do_func()