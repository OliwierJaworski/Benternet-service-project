article: https://medium.com/data-science/llama-cpp-writing-a-simple-c-inference-program-for-gguf-llm-models-12bc5f58505f
content: 
- detailed look on how LLM inference is performed using low-level functions from llama.cpp
- simple chat bot -> based on SmolChat example
- llama.cpp constructs

https://github.com/ggml-org/llama.cpp/blob/master/examples/simple-chat/simple-chat.cpp
https://github.com/ggml-org/llama.cpp/tree/b4902

updating cmake version from 3.10.2 to 3.19.1
https://forums.developer.nvidia.com/t/how-does-jetson-nono-update-cmake-to-3-18/182786/2
https://www.reddit.com/r/LocalLLaMA/comments/11yi0bl/build_llamacpp_on_jetson_nano_2gb/
https://mudler.pm/posts/local-ai-jetson-nano-devkit/
https://huggingface.co/Pi3141/alpaca-7b-native-enhanced
https://github.com/ggml-org/llama.cpp/issues/4099
https://gist.github.com/FlorSanders/2cf043f7161f52aa4b18fb3a1ab6022f

https://medium.com/@aalokpatwa/optimizing-llm-inference-managing-the-kv-cache-34d961ead936

Use recurrent-style architectures (like RNNs) if maintaining long-term memory is needed.

https://medium.com/@RobuRishabh/recurrent-neural-network-rnn-8412b9abd755

sliding window attention  -> token threshold 

cuda stuff
--- non persistant across reboots ---
export PATH="/usr/local/cuda-12.8/bin:$PATH"
export LD_LIBRARY_PATH="/usr/local/cuda-12.8/lib64:$LD_LIBRARY_PATH"

--- persistant across reboots ---
echo 'export PATH="/usr/local/cuda-12.8/bin:$PATH"' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH="/usr/local/cuda-12.8/lib64:$LD_LIBRARY_PATH"' >> ~/.bashrc
source ~/.bashrc

https://www.reddit.com/r/cpp/comments/11r9xni/build_systems_for_c/?rdt=44492
https://github.com/bitmeal/vcpkg-cmake-integration