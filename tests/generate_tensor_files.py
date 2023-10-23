import torch

tensors = [
    ([1, 1, 55], "x1_in.txt"),
    ([1, 1, 55, 10], "unnormalized_widths.txt"), 
    ([1, 1, 55, 10], "unnormalized_heights.txt"),
    ([1, 1, 55, 9], "unnormalized_derivatives.txt")
]

def save_tensor_to_file(tensor: torch.Tensor, file_path: str) -> None:
    tensor_list = tensor.flatten().tolist()
    tensor_str = [str(val) for val in tensor_list]
    tensor_str = ", ".join(tensor_str)

    with open(file_path, 'w') as file:
        file.write(tensor_str)

for tensor_shape, name in tensors:
    folder = "tensor_files/"
    tensor = torch.randn(*tensor_shape)
    save_tensor_to_file(tensor, folder + name)