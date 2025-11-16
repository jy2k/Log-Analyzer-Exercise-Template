# OpenAI Chat Client in C

A simple C program that demonstrates how to make requests to the OpenAI API using libcurl. The program sends a "hello" message to the ChatGPT API and displays the response.

## Features

- Makes HTTP POST requests to OpenAI's Chat Completions API
- Supports multiple methods for API key configuration
- JSON request/response handling
- Dynamic memory allocation for responses
- Error handling and validation

## Prerequisites

- **libcurl**: Required for HTTP requests
- **GCC**: For compilation
- **OpenAI API Key**: Required for API access
- **Context7 API Key**: Optional, for enhanced documentation access

## Installation

### macOS
```bash
# libcurl is usually pre-installed on macOS
# If not available, install via Homebrew:
brew install curl
```

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev build-essential
```

### Linux (CentOS/RHEL)
```bash
sudo yum install libcurl-devel gcc
```

## API Key Setup

### Method 1: File-based Configuration (Recommended)

1. **OpenAI API Key**:
   ```bash
   echo "your-openai-api-key-here" > api_key.txt
   ```

2. **Context7 API Key** (Optional - for enhanced documentation):
   - Edit `.vscode/mcp.json` and update the Context7 API key:
   ```json
   {
     "servers": {
       "context7": {
         "type": "http",
         "url": "https://mcp.context7.com/mcp",
         "headers": {
           "CONTEXT7_API_KEY": "your-context7-api-key-here"
         }
       }
     }
   }
   ```

## Compilation and Usage

1. **Compile the program**:
   ```bash
   gcc -o openai_chat openai_chat.c -lcurl
   ```

2. **Run the program**:
   ```bash
   ./openai_chat
   ```

## Example Output

```
✓ API key loaded from api_key.txt
Sending request to OpenAI API with message: "hello"
Request payload: {"model": "gpt-3.5-turbo","messages": [{"role": "user","content": "hello"}],"max_tokens": 100}

Response from OpenAI API:
{
  "id": "chatcmpl-xyz123",
  "object": "chat.completion",
  "created": 1699123456,
  "model": "gpt-3.5-turbo-0125",
  "choices": [
    {
      "index": 0,
      "message": {
        "role": "assistant",
        "content": "Hello! How can I assist you today?",
        "refusal": null
      },
      "finish_reason": "stop"
    }
  ],
  "usage": {
    "prompt_tokens": 8,
    "completion_tokens": 9,
    "total_tokens": 17
  }
}
```

## File Structure

```
Chaos-c-openAI/
├── openai_chat.c       # Main C source file
├── openai_chat         # Compiled binary
├── api_key.txt         # OpenAI API key (create this)
├── .vscode/
│   └── mcp.json        # Context7 MCP configuration
├── README.md           # This file
└── .gitignore          # Git ignore rules
```
