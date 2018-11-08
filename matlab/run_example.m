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

tree = HDC();
child = HDC();
child.set_data(1.23456);
tree.add("aaa/bbb",child);
tree.dump()
child2 = tree.get("aaa/bbb");
child2.dump()

list = HDC();
child = HDC();
child.set_data(1.23456);
child2 = HDC();
child2.set_data(2.22222);
list.append(child);
list.append(child);
list.append(child);
list.dump()
ch = list.get(1);
ch.dump()
list.insert(1,child2);
list.dump()
list.set(1,child);
list.dump()

%str = HDC("aaa");
%str.dump()
d = HDC(1.23456789987);
d.dump()

d2 = HDC([111,2,3]);
d2.dump();
aaa = d2.get_data()

f = HDC(single(111.));
f.dump()
i8 = HDC(int8(-8));
i8.dump()

str = HDC();
str.set_data('aaaa')
disp("set str done")
str.dump()
a = str.get_data()

%l = HDC.load_json("tree.json","");
%l.dump()

tree = HDC();
child = HDC();
child.set_data([1.23456]);
tree.add("aaa/bbb",child);
aaa = child.get_data()
child2 = tree.at("aaa/bbb")
subtree = tree.at("aaa")
%}

tree = HDC();
tree.set("aaa/bbb",3.141592)
ch = HDC("jbgfaigigbaibi")
tree.set("aaa/ccc",ch)
tree.dump()
tree.delete_child("aaa/bbb")
tree.dump()
