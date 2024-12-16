### **README.md**

```markdown
# 🌐 Servidor HTTP em C++ 🚀

Bem-vindo ao **Servidor HTTP** desenvolvido em **C++**, uma solução simples, eficiente e ideal para aprender sobre os fundamentos de redes, sockets e manipulação de arquivos. Este servidor suporta requisições HTTP básicas, como **GET** e **POST**, e serve arquivos locais diretamente para o navegador ou clientes como `curl`.

---

## 🎯 **Funcionalidades**
- **Requisições HTTP**:
  - Suporte aos métodos **GET** e **POST**.
- **Servidor Multithread**:
  - Processa várias conexões simultâneas.
- **Serviço de Arquivos Locais**:
  - Retorna arquivos estáticos como HTML, CSS, JS, imagens e mais.
- **Proteção contra Path Traversal**:
  - Segurança embutida para evitar acessos indevidos ao sistema de arquivos.
- **Registro de Logs**:
  - Todos os acessos são registrados no arquivo `server.log`.

---

## 🛠️ **Como Usar**

### **1. Configuração**
1. Certifique-se de ter o compilador **g++** instalado e funcionando.
2. Clone este repositório:
   ```bash
   git clone https://github.com/Dyogo199/ServidorHTTP.git
   cd ServidorHTTP
   ```

### **2. Compilação**
Compile o código com:
```bash
g++ -o ServidorHTTP main.cpp -lws2_32
```

### **3. Execução**
Inicie o servidor:
```bash
./ServidorHTTP.exe
```
O servidor estará disponível na porta **9090** por padrão.

---

## 🔍 **Testando o Servidor**

### **1. Acesse pelo Navegador**
Digite no navegador:
```
http://localhost:9090
```
Se houver um arquivo `index.html` no diretório do servidor, ele será exibido.

### **2. Teste com `curl`**
- **GET**:
  ```bash
  curl http://localhost:9090/index.html
  ```
- **POST**:
  ```bash
  curl -X POST -d "key=value" http://localhost:9090
  ```

---

## 📂 **Estrutura do Projeto**
```
.
├── main.cpp         # Código-fonte principal do servidor
├── index.html       # Página inicial (opcional)
├── server.log       # Arquivo de logs
```

---

## 🔒 **Segurança**
- **Proteção contra Path Traversal**: Bloqueia caminhos que tentem acessar arquivos fora do diretório permitido (`../`).
- **Logs Detalhados**: Todos os acessos são registrados no `server.log`, facilitando auditorias.

---

## 🌟 **Destaques Técnicos**
- **Multithreading**: Threads independentes para gerenciar conexões simultâneas.
- **WinSock no Windows**: Usa a biblioteca WinSock para garantir compatibilidade.
- **Manipulação de Arquivos**: Carrega e envia arquivos estáticos de forma eficiente.

---

## 📚 **Próximas Melhorias**
1. Adicionar suporte para HTTPS usando OpenSSL.
2. Suporte a mais métodos HTTP (PUT, DELETE, etc.).
3. Cache de arquivos para melhorar a performance.

---

## 💡 **Aprendizados**
Este projeto é uma oportunidade de entender conceitos fundamentais:
- **Sockets** e comunicação cliente-servidor.
- Manipulação de **strings**, **arquivos** e **conexões de rede**.
- Boas práticas de desenvolvimento em **C++**.

---

## ✍️ **Contribuições**
Contribuições são bem-vindas! Sinta-se à vontade para abrir uma issue ou enviar um pull request.

---

## 🏆 **Licença**
Distribuído sob a licença MIT. Consulte o arquivo `LICENSE` para mais detalhes.

---

🌐 **Servidor HTTP**: um projeto simples, mas poderoso para aprender e explorar as bases do desenvolvimento de servidores. Experimente, aprenda e crie! 🚀
```

---



Se precisar ajustar algo ou adicionar mais detalhes, me avise! 😊
