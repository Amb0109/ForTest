c_test = test()
print(c_test:is_test())
print(c_test:is_base())
c_test:is_what()

c_test:set_xyz(1.0, 2.5, 4.0)
c_pos = c_test:get_xyz2()

print("x=" .. c_pos.x .. " y=" .. c_pos.y .. " z=" .. c_pos.z)