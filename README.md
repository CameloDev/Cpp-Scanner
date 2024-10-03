# Projeto Avançado com ImGui, cURL e Ferramentas Avançadas

Este é um projeto avançado em C++ desenvolvido no Visual Studio, utilizando o [ImGui](https://github.com/ocornut/imgui) para a interface gráfica e o [cURL](https://curl.se/) para realizar requisições HTTP. Além disso, o projeto inclui um **string extractor em Python**, usado para extrair strings específicas de arquivos, e um **scanner que interage com processos do sistema**, listando essas informações em uma API. O projeto foi interrompido devido à falta de tempo, mas é publicado aqui para demonstrar o progresso alcançado.

## Visão Geral do Projeto

O projeto combina várias tecnologias para criar uma ferramenta avançada de interação entre uma interface gráfica personalizada e o sistema subjacente, além de comunicações com APIs externas. O objetivo era permitir que usuários visualizassem informações extraídas do sistema e de APIs em tempo real, tudo através de uma interface simples e flexível.

### Principais Funcionalidades

- **Interface Gráfica (ImGui)**: Desenvolvida com ImGui, proporcionando uma interface leve e customizável para o usuário interagir com o backend.
- **Requisições HTTP (cURL)**: Utiliza cURL para realizar requisições GET e POST, possibilitando comunicação com APIs externas e servidores.
- **String Extractor (Python)**: Um módulo Python integrado ao projeto que extrai strings específicas de arquivos. Pode ser usado para buscar informações textuais relevantes de diversos tipos de arquivos no sistema.
- **Scanner de Processos do Sistema**: O scanner examina processos em execução no sistema operacional, coletando informações sobre eles e listando esses dados em uma API. Isso permite monitoramento e análise do comportamento de processos diretamente através da interface gráfica.
- **Modularidade**: O código foi estruturado para facilitar a adição de novos componentes, tanto na interface quanto na lógica de comunicação com o sistema e APIs.
- **Suporte a Multi-threading**: Lida com múltiplas requisições HTTP e operações de sistema de forma assíncrona, garantindo uma interface sempre responsiva.
- **Tratamento Básico de Erros**: Implementação inicial de tratamento de erros, incluindo a falha de requisições HTTP e a manipulação de processos no sistema.

### Requisitos

Para compilar e executar o projeto, você precisará de:

- Visual Studio 2019 (ou mais recente) com suporte para desenvolvimento C++.
- Python 3.x para o string extractor.
- ImGui (integrado ao projeto como submódulo ou incluído manualmente).
- Biblioteca cURL devidamente vinculada ao projeto.
- Acesso ao sistema para monitoramento de processos.

### Como Compilar o Projeto

1. Clone o repositório em sua máquina local:
   ```bash
   git clone https://github.com/camelodev/scanner.git
