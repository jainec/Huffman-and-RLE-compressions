# Huffman-and-RLE-compressions

Esta é uma implementação em C dos algoritmos de compressão Huffman e RLE aplicados no seguinte problema:

> A empresa de telecomunicações Poxim Tech está desenvolvendo um sistema para compressão de dados para minimizar a utilização de banda na transmissão dos dados, avaliando qual técnica apresenta a melhor taxa de compressão

> São fornecidas sequências de bytes em formato hexadecimal que possuem valores entre 0x00 até 0xFF, com tamanho máximo de 10000 caracteres.

> As codificações de 8 bits Run-Length Encoding (RLE) e de Huffman (HUF) são utilizadas para compressão.

> A técnica que apresentar menor quantidade de bytes é selecionada para a transmissão dos dados.

# Formato de entrada

> [#Quantidade de sequências]

> [#T1] [B1 . . . Bn]

.

.

> [#TN ] [BN . . . BNm]

**Exemplo:**

		4	
		5 0xAA 0xAA 0xAA 0xAA 0xAA
		7 0x10 0x20 0x30 0x40 0x50 0x60 0x70
		9 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
		4 0xFA 0xFA 0xC1 0xC1

# Formato da saída

> Cada linha da saída gerada deve conter o algoritmo utilizado na compressão dos dados (RLE ou HUF) e o valor da taxa de compressão é um número real com duas casas decimais de precisão.

> Em uma situação onde ambos as técnicas apresentarem o mesmo número de bytes na codificação, devem ser impressas ambas as saídas,
seguindo a ordem HUF e RLE.

**Exemplo:**

	[HUF 20.00%] 0x00
	[HUF 42.86%] 0x9C 0x6C 0x50
	[HUF 22.22%] 0x00 0x00
	[RLE 22.22%] 0x09 0xFF
	[HUF 25.00%] 0xC0
