%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% test constructor %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
obj = HDC();
assert(class(obj) == "HDC")

s = 1.23;
obj = HDC(s);
assert(obj.get_data() == s)

s = int8(-8);
obj = HDC(s);
assert(obj.get_data() == s)
assert(isa(obj.get_data(),'int8'))

s = 1:5;
obj = HDC(s);
assert(all(obj.get_data() == s))

s = int16(1:5);
obj = HDC(s);
assert(all(obj.get_data() == s))
assert(isa(obj.get_data(),'int16'))

s = reshape(1:10,2,5);
obj = HDC(s);
s2 = obj.get_data();
assert(all(size(s2) == size(s)))
assert(all(s2(:) == s(:)))

s = "aaa";
obj = HDC(s);
assert(obj.get_data() == s)

s = 'bbb';
obj = HDC(s);
assert(any(obj.get_data() == s))



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% test set / get %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
s = 1.23;
obj = HDC(s);
assert(obj.get_data() == s)


s = 1:5;
obj.set_data(s);
assert(all(obj.get_data() == s))

s = int16(1:5);
obj.set_data(s);
assert(all(obj.get_data() == s))
assert(isa(obj.get_data(),'int16'))

s = reshape(1:10,2,5);
obj.set_data(s);
s2 = obj.get_data();
assert(all(size(s2) == size(s)))
assert(all(s2(:) == s(:)))

s = "aaa";
obj.set_data(s);
assert(obj.get_data() == s)

s = 'bbb';
obj.set_data(s);
assert(all(obj.get_data() == s))



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% test node manipulation %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tree = HDC();
s = 999;
child = HDC(s);
path = "aaa/bbb";

% test add
tree.add_child(path,child);
child2 = tree.get_child(path);
assert(child2.get_data() == s)
assert(tree.at(path) == s)

%test set
r = 666;
child2 = HDC(r);
tree.set(path,child2);
assert(child2.get_data() == r)
assert(tree.at(path) == r)

%test list
list = HDC();
s = 1.23456;
s2 = 2.22222;
ch = HDC(s);
ch2 = HDC(s2);
list.append(ch);
list.append(ch);
list.append(ch);
ch_ = list.at(1);
assert(list.at(2) == s)
list.insert(2,ch2);
assert(list.at(2) == s2)
assert(list.at(3) == s)

%test add, set and keys
tree = HDC();
s = 1.23456;
s2 = 2.22222;
ch = HDC(s);
ch2 = HDC(s2);
tree.add_child("aaa",ch)
assert(tree.shape() == 1)
tree.add_child("bbb",ch)
assert(tree.shape() == 2)
expected_keys = {'aaa';'bbb'};
keys = tree.keys();
for i=1:length(keys)
    assert(all(keys{i} == expected_keys{i}))
end

%test set/at
tree = HDC();
tree.set("aaa/bbb",3.141592)
a = tree.at("aaa/bbb");
assert(a == 3.141592)
assert(isa(a,'double'))
c = tree.at("aaa");
assert(isa(c,'HDC'))
tree.set("bbb",45645656456)
assert(tree.exists("bbb"))
tree.delete_child("bbb");
assert(~tree.exists("bbb"))


% test copy
tree2 = tree.copy();
assert(~all(tree.get_child("aaa/bbb").as_hdc_t.uuid == tree2.get_child("aaa/bbb").as_hdc_t.uuid))
assert(tree.at("aaa/bbb") == tree2.at("aaa/bbb"))

% test json
uri = join(["json://",pwd,"/test.json"],"");
tree.save(uri);
tree2 = HDC.load(uri);
assert(tree.at("aaa/bbb") == tree2.at("aaa/bbb"))

% test json - data_path specified
uri = join(["json://",pwd,"/test2.json"],"");
data_path = "data";
tree.save(uri,data_path);
tree2 = HDC.load(uri,data_path);
assert(tree.at("aaa/bbb") == tree2.at("aaa/bbb"))


disp("All tests are OK...")

exit

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% TODO %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% HDF5
% more sizes / types + class strings, errors when type not known, objects / cell arrays???
