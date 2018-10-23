%{
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
%}

tree = HDC();
child = HDC();
child.set_data(1.23456);
tree.add_child("aaa/bbb",child);
tree.dump()
child2 = tree.get_child("aaa/bbb");
child2.dump()
