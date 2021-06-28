//#include <torch/script.h>
//#include <iostream>
//#include <vector>
//int TorchTest() {
//	torch::jit::script::Module module = torch::jit::load("model.pt");
//	std::vector<torch::jit::IValue> inputs;
//	inputs.push_back(torch::ones({ 3,3 }));
//	auto out = module.forward(inputs).toTensor();
//	
//	std::cout << out << std::endl;
//	std::vector<float> tt = { 1.0,2.0,3.0,4.0 };
//	auto t = torch::from_blob(tt.data(), { 4 });
//	std::cout << t << std::endl;
//	char c;
//	std::cin >> c;
//	return 0;
//}