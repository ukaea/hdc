addpath('../../build/matlab');

equilibrium = HDC()
equilibrium.set('profiles_1d/psi', linspace(0, 1, 5))
equilibrium.set('time',2.34)

equilibrium.dump()
equilibrium.at('profiles_1d/psi')

disp(["equilibrium['time'] -> ",mat2str(equilibrium.at("time"))])
disp(["equilibrium['profiles_1d/psi'] -> {}",mat2str(equilibrium.at('profiles_1d/psi'))])

tree_out = test_cpos(equilibrium)

if all(tree_out.at('distsourceout/source/profiles_1d/psi') == -3 * equilibrium.at('profiles_1d/psi'))
    disp("Data ok.")
else
    disp("Different data! Something bad has happened.")
end
