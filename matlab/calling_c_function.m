equilibrium = HDC()
equilibrium.set('profiles_1d/psi', linspace(0, 1, 5))
equilibrium.set('time',2.34)

equilibrium.dump()

disp(["equilibrium['time'] -> ",num2str(equilibrium.at("time"))])
disp(["equilibrium['profiles_1d/psi'] -> {}",num2str(equilibrium.at('profiles_1d/psi'))])

tree_out = test_cpos(equilibrium)

tree_out.dump()

tree_out.at('distsourceout/source/profiles_1d/psi') == -3 * equilibrium.at('profiles_1d/psi')
