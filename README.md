# Projeto Avançado com ImGui e cURL

Este é um projeto avançado em C++ desenvolvido no Visual Studio, utilizando o [ImGui](https://github.com/ocornut/imgui) para a interface gráfica e o [cURL](https://curl.se/) para realizar requisições HTTP. O projeto foi iniciado com objetivos ambiciosos, combinando uma interface de usuário interativa com comunicação de servidor em tempo real. No entanto, devido à falta de tempo, o projeto não foi concluído. Ele está sendo disponibilizado aqui para demonstrar o progresso e a arquitetura complexa construída.

## Visão Geral do Projeto

O objetivo deste projeto é demonstrar a integração de uma interface de usuário altamente customizável (ImGui) com funcionalidades de comunicação em rede (cURL). A ideia era criar uma ferramenta em que os usuários pudessem interagir com dados obtidos de APIs externas em tempo real, visualizando essas informações e gerenciando configurações através de uma interface gráfica.

### Principais Funcionalidades

- **Interface Gráfica**: Desenvolvida com ImGui, oferecendo uma interface leve e flexível para interagir com o backend.
- **Requisições HTTP**: Utiliza cURL para enviar e receber dados de APIs externas, suportando tanto requisições GET quanto POST.
- **Design Modular**: O código foi estruturado de forma a facilitar a adição de novos elementos de UI e a extensão das funcionalidades de requisição HTTP.
- **Suporte a Multi-threading**: Projetado para lidar com múltiplas requisições HTTP de forma assíncrona, garantindo que a UI permaneça responsiva.
- **Tratamento de Erros**: Implementação básica para lidar com requisições de rede falhas ou respostas inválidas.

### Requisitos

Para compilar e executar o projeto, você vai precisar de:

- Visual Studio 2019 (ou mais recente) com o ambiente de desenvolvimento C++ instalado.
- ImGui (integrado ao projeto como submódulo ou incluído manualmente).
- Biblioteca cURL instalada e devidamente vinculada ao projeto.

### Como Compilar o Projeto

1. Clone o repositório em sua máquina local:
   ```bash
   git clone https://github.com/camelodev/scanner.git
