obj = HDC();

obj.set_data(1.23);
obj.dump()

obj.set_data([1, 2, 3; 4, 5, 6]);
obj.dump()

y = obj.get_data()

% clear obj
