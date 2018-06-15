disp("constructor");
obj = HDC();
disp("constructor done");
obj.dump();
disp("dump done");
obj.set_data(1.23);
disp("assign done");
obj.dump()
disp("dump done");
obj.set_data([1, 2, 3; 4, 5, 6]);
disp("array assign done");
obj.dump()
disp("dump done");
y = obj.get_data()
disp("get data done");
% clear obj
