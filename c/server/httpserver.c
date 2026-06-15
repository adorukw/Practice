#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 8192
#define MAX_PATH 256

// 响应状态码和描述
const char* get_status_message(int code) {
    switch(code) {
        case 200: return "OK";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 400: return "Bad Request";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 500: return "Internal Server Error";
        default: return "Unknown";
    }
}

// 获取文件扩展名对应的MIME类型
const char* get_mime_type(const char* path) {
    const char* ext = strrchr(path, '.');
    if (!ext) return "text/plain";
    
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) 
        return "text/html; charset=utf-8";
    if (strcmp(ext, ".css") == 0) 
        return "text/css";
    if (strcmp(ext, ".js") == 0) 
        return "application/javascript";
    if (strcmp(ext, ".json") == 0) 
        return "application/json";
    if (strcmp(ext, ".png") == 0) 
        return "image/png";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) 
        return "image/jpeg";
    if (strcmp(ext, ".gif") == 0) 
        return "image/gif";
    if (strcmp(ext, ".pdf") == 0) 
        return "application/pdf";
    if (strcmp(ext, ".txt") == 0) 
        return "text/plain";
    if (strcmp(ext, ".svg") == 0) 
        return "image/svg+xml";
    
    return "application/octet-stream";
}

// 发送HTTP响应头
void send_headers(int client_fd, int status, const char* mime_type, long content_length) {
    char buffer[1024];
    time_t now = time(NULL);
    struct tm* tm_info = gmtime(&now);
    char time_buf[100];
    strftime(time_buf, sizeof(time_buf), "%a, %d %b %Y %H:%M:%S GMT", tm_info);
    
    int len = snprintf(buffer, sizeof(buffer),
        "HTTP/1.1 %d %s\r\n"
        "Server: C-HTTP-Server/1.0\r\n"
        "Date: %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n",
        status, get_status_message(status), time_buf, mime_type, content_length);
    
    write(client_fd, buffer, len);
}

// 发送错误页面
void send_error(int client_fd, int status, const char* message) {
    char body[BUFFER_SIZE];
    int len = snprintf(body, sizeof(body),
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>%d %s</title></head>"
        "<body>"
        "<h1>%d %s</h1>"
        "<p>%s</p>"
        "<hr>"
        "<p><i>C HTTP Server</i></p>"
        "</body>"
        "</html>",
        status, get_status_message(status), status, get_status_message(status), message);
    
    send_headers(client_fd, status, "text/html; charset=utf-8", len);
    write(client_fd, body, len);
}

// 发送目录列表
void send_directory_listing(int client_fd, const char* path, const char* url_path) {
    char body[BUFFER_SIZE * 4];
    char* body_ptr = body;
    
    body_ptr += sprintf(body_ptr,
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<title>Index of %s</title>"
        "<style>"
        "body { font-family: Arial, sans-serif; margin: 40px; }"
        "h1 { color: #333; }"
        "table { border-collapse: collapse; width: 100%%; }"
        "th, td { padding: 8px; text-align: left; border-bottom: 1px solid #ddd; }"
        "tr:hover { background-color: #f5f5f5; }"
        "</style>"
        "</head>"
        "<body>"
        "<h1>Index of %s</h1>"
        "<table>"
        "<tr><th>Name</th><th>Size</th><th>Modified</th></tr>",
        url_path, url_path);
    
    // 父目录链接
    if (strcmp(url_path, "/") != 0) {
        body_ptr += sprintf(body_ptr, 
            "<tr><td><a href=\"..\">../</a></td><td>-</td><td>-</td></tr>");
    }
    
    // 读取目录
    char full_path[MAX_PATH];
    snprintf(full_path, sizeof(full_path), ".%s", path);
    
    DIR* dir = opendir(full_path);
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0) continue;
            
            char entry_path[MAX_PATH];
            snprintf(entry_path, sizeof(entry_path), "%s/%s", full_path, entry->d_name);
            
            struct stat statbuf;
            if (stat(entry_path, &statbuf) == 0) {
                char size_str[32];
                char time_str[64];
                const char* name = entry->d_name;
                
                if (S_ISDIR(statbuf.st_mode)) {
                    strcpy(size_str, "-");
                    name = strcat(strcpy(malloc(strlen(entry->d_name) + 2), entry->d_name), "/");
                } else {
                    if (statbuf.st_size < 1024) {
                        sprintf(size_str, "%ld B", statbuf.st_size);
                    } else if (statbuf.st_size < 1024 * 1024) {
                        sprintf(size_str, "%.1f KB", statbuf.st_size / 1024.0);
                    } else {
                        sprintf(size_str, "%.1f MB", statbuf.st_size / (1024.0 * 1024.0));
                    }
                }
                
                struct tm* tm_info = localtime(&statbuf.st_mtime);
                strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", tm_info);
                
                char encoded_name[256];
                char* src = entry->d_name;
                char* dst = encoded_name;
                while (*src && dst - encoded_name < 250) {
                    if (*src == ' ') *dst++ = '+';
                    else if (*src == '&') { strcpy(dst, "&amp;"); dst += 5; }
                    else if (*src == '<') { strcpy(dst, "&lt;"); dst += 4; }
                    else if (*src == '>') { strcpy(dst, "&gt;"); dst += 4; }
                    else if (*src == '"') { strcpy(dst, "&quot;"); dst += 6; }
                    else if (*src == '\'') { strcpy(dst, "&#39;"); dst += 5; }
                    else *dst++ = *src;
                    src++;
                }
                *dst = '\0';
                
                body_ptr += sprintf(body_ptr,
                    "<tr><td><a href=\"%s\">%s</a></td><td>%s</td><td>%s</td></tr>",
                    encoded_name, name, size_str, time_str);
            }
        }
        closedir(dir);
    }
    
    strcpy(body_ptr, 
        "</table>"
        "<hr>"
        "<p><i>C HTTP Server - Directory Listing</i></p>"
        "</body>"
        "</html>");
    
    send_headers(client_fd, 200, "text/html; charset=utf-8", strlen(body));
    write(client_fd, body, strlen(body));
}

// 解析HTTP请求
void parse_request(const char* request, char* method, char* path) {
    sscanf(request, "%s %s", method, path);
    
    // 解码URL编码
    char* src = path;
    char* dst = path;
    while (*src) {
        if (*src == '%' && src[1] && src[2]) {
            int hex;
            sscanf(src + 1, "%2x", &hex);
            *dst++ = hex;
            src += 3;
        } else if (*src == '+') {
            *dst++ = ' ';
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

// 处理客户端连接
void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
    
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        
        char method[16], path[MAX_PATH];
        parse_request(buffer, method, path);
        
        printf("请求: %s %s\n", method, path);
        
        // 只处理GET请求
        if (strcmp(method, "GET") != 0) {
            send_error(client_fd, 405, "Method Not Allowed");
            close(client_fd);
            return;
        }
        
        // 防止路径遍历攻击
        if (strstr(path, "..")) {
            send_error(client_fd, 403, "Forbidden");
            close(client_fd);
            return;
        }
        
        // 如果路径是目录，添加index.html
        char full_path[MAX_PATH];
        if (strcmp(path, "/") == 0) {
            snprintf(full_path, sizeof(full_path), "./index.html");
        } else {
            snprintf(full_path, sizeof(full_path), ".%s", path);
        }
        
        struct stat statbuf;
        if (stat(full_path, &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {
                // 如果是目录，检查是否有index.html
                char index_path[MAX_PATH];
                snprintf(index_path, sizeof(index_path), "%s/index.html", full_path);
                
                if (stat(index_path, &statbuf) == 0) {
                    strcpy(full_path, index_path);
                } else {
                    // 如果没有index.html，显示目录列表
                    send_directory_listing(client_fd, path, path);
                    close(client_fd);
                    return;
                }
            }
            
            // 打开文件
            int file_fd = open(full_path, O_RDONLY);
            if (file_fd < 0) {
                send_error(client_fd, 403, "Forbidden");
            } else {
                // 发送文件
                off_t file_size = lseek(file_fd, 0, SEEK_END);
                lseek(file_fd, 0, SEEK_SET);
                
                send_headers(client_fd, 200, get_mime_type(full_path), file_size);
                
                char file_buffer[8192];
                ssize_t bytes;
                while ((bytes = read(file_fd, file_buffer, sizeof(file_buffer))) > 0) {
                    write(client_fd, file_buffer, bytes);
                }
                
                close(file_fd);
            }
        } else {
            send_error(client_fd, 404, "File Not Found");
        }
    }
    
    close(client_fd);
}

// 客户端处理线程
void* client_thread(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);
    
    handle_client(client_fd);
    
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // 创建socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket创建失败");
        exit(EXIT_FAILURE);
    }
    
    // 设置socket选项
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // 绑定地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("绑定失败");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 开始监听
    if (listen(server_fd, 10) < 0) {
        perror("监听失败");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("================================================================\n");
    printf("C语言HTTP服务器已启动！\n");
    printf("访问地址: http://localhost:%d\n", PORT);
    printf("================================================================\n");
    printf("服务器特性:\n");
    printf("• 支持静态文件服务 (HTML, CSS, JS, 图片等)\n");
    printf("• 自动目录列表\n");
    printf("• 支持MIME类型\n");
    printf("• 多线程处理\n");
    printf("• 基本的错误处理\n");
    printf("================================================================\n");
    printf("按Ctrl+C停止服务器\n");
    printf("================================================================\n");
    
    // 创建示例文件和目录
    system("mkdir -p static");
    system("mkdir -p static/images");
    
    // 创建示例HTML文件
    FILE* html = fopen("index.html", "w");
    if (html) {
        fprintf(html,
            "<!DOCTYPE html>\n"
            "<html lang=\"zh-CN\">\n"
            "<head>\n"
            "    <meta charset=\"UTF-8\">\n"
            "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
            "    <title>C语言HTTP服务器</title>\n"
            "    <style>\n"
            "        * { margin: 0; padding: 0; box-sizing: border-box; }\n"
            "        body { \n"
            "            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;\n"
            "            line-height: 1.6;\n"
            "            color: #333;\n"
            "            background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%);\n"
            "            min-height: 100vh;\n"
            "        }\n"
            "        .container {\n"
            "            max-width: 800px;\n"
            "            margin: 0 auto;\n"
            "            padding: 2rem;\n"
            "        }\n"
            "        .card {\n"
            "            background: white;\n"
            "            border-radius: 20px;\n"
            "            padding: 3rem;\n"
            "            box-shadow: 0 20px 60px rgba(0,0,0,0.3);\n"
            "            animation: slideIn 0.5s ease-out;\n"
            "        }\n"
            "        @keyframes slideIn {\n"
            "            from { opacity: 0; transform: translateY(20px); }\n"
            "            to { opacity: 1; transform: translateY(0); }\n"
            "        }\n"
            "        h1 {\n"
            "            color: #667eea;\n"
            "            margin-bottom: 1.5rem;\n"
            "            font-size: 2.5rem;\n"
            "            border-bottom: 3px solid #667eea;\n"
            "            padding-bottom: 1rem;\n"
            "        }\n"
            "        h2 { color: #764ba2; margin: 2rem 0 1rem; }\n"
            "        .feature-grid {\n"
            "            display: grid;\n"
            "            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));\n"
            "            gap: 1.5rem;\n"
            "            margin: 2rem 0;\n"
            "        }\n"
            "        .feature {\n"
            "            background: #f7f7ff;\n"
            "            padding: 1.5rem;\n"
            "            border-radius: 10px;\n"
            "            border-left: 4px solid #667eea;\n"
            "        }\n"
            "        .feature h3 {\n"
            "            color: #667eea;\n"
            "            margin-bottom: 0.5rem;\n"
            "        }\n"
            "        .btn {\n"
            "            display: inline-block;\n"
            "            background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%);\n"
            "            color: white;\n"
            "            padding: 0.8rem 2rem;\n"
            "            border-radius: 50px;\n"
            "            text-decoration: none;\n"
            "            font-weight: bold;\n"
            "            margin: 1rem 0;\n"
            "            transition: all 0.3s ease;\n"
            "            border: none;\n"
            "            cursor: pointer;\n"
            "        }\n"
            "        .btn:hover {\n"
            "            transform: translateY(-2px);\n"
            "            box-shadow: 0 10px 20px rgba(102, 126, 234, 0.3);\n"
            "        }\n"
            "        code {\n"
            "            background: #f1f1f1;\n"
            "            padding: 0.2rem 0.5rem;\n"
            "            border-radius: 4px;\n"
            "            font-family: 'Courier New', monospace;\n"
            "        }\n"
            "        .demo-area {\n"
            "            margin-top: 2rem;\n"
            "            padding: 1.5rem;\n"
            "            background: #f8f9fa;\n"
            "            border-radius: 10px;\n"
            "        }\n"
            "        .links {\n"
            "            display: flex;\n"
            "            gap: 1rem;\n"
            "            flex-wrap: wrap;\n"
            "            margin-top: 2rem;\n"
            "        }\n"
            "        .footer {\n"
            "            margin-top: 3rem;\n"
            "            padding-top: 1.5rem;\n"
            "            border-top: 2px solid #eee;\n"
            "            text-align: center;\n"
            "            color: #666;\n"
            "        }\n"
            "    </style>\n"
            "</head>\n"
            "<body>\n"
            "    <div class=\"container\">\n"
            "        <div class=\"card\">\n"
            "            <h1>🚀 C语言HTTP服务器</h1>\n"
            "            <p style=\"font-size: 1.2rem; color: #666; margin-bottom: 2rem;\">\n"
            "                这是一个完全用C语言编写的HTTP服务器，仅使用标准库和系统调用！\n"
            "            </p>\n"
            "            \n"
            "            <div class=\"feature-grid\">\n"
            "                <div class=\"feature\">\n"
            "                    <h3>⚡️ 高性能</h3>\n"
            "                    <p>直接使用系统调用，无额外开销，极致性能。</p>\n"
            "                </div>\n"
            "                <div class=\"feature\">\n"
            "                    <h3>🔧 原生实现</h3>\n"
                    "                    <p>仅使用C标准库和POSIX API，无需第三方库。</p>\n"
            "                </div>\n"
            "                <div class=\"feature\">\n"
            "                    <h3>📁 文件服务</h3>\n"
            "                    <p>支持静态文件服务和自动目录列表。</p>\n"
            "                </div>\n"
            "                <div class=\"feature\">\n"
            "                    <h3>🔄 多线程</h3>\n"
            "                    <p>每个连接独立线程，支持高并发。</p>\n"
            "                </div>\n"
            "            </div>\n"
            "            \n"
            "            <h2>🛠️ 服务器信息</h2>\n"
            "            <ul style=\"margin-left: 1.5rem; margin-bottom: 2rem;\">\n"
            "                <li><strong>端口：</strong><code>%d</code></li>\n"
            "                <li><strong>协议：</strong>HTTP/1.1</li>\n"
            "                <li><strong>线程模型：</strong>每连接一线程</li>\n"
            "                <li><strong>支持文件类型：</strong>HTML, CSS, JS, 图片等</li>\n"
            "            </ul>\n"
            "            \n"
            "            <div class=\"demo-area\">\n"
            "                <h2>🔍 功能演示</h2>\n"
            "                <div class=\"links\">\n"
            "                    <a href=\"/\" class=\"btn\">🏠 首页</a>\n"
            "                    <a href=\"/static/\" class=\"btn\">📁 静态文件夹</a>\n"
            "                    <a href=\"/notfound\" class=\"btn\">❌ 404页面</a>\n"
            "                    <a href=\"https://github.com\" class=\"btn\">🌐 外部链接</a>\n"
            "                </div>\n"
            "                \n"
            "                <p style=\"margin-top: 1rem;\">尝试在static文件夹中创建文件，服务器会自动显示。</p>\n"
            "            </div>\n"
            "            \n"
            "            <h2>💻 技术栈</h2>\n"
            "            <p>这个服务器使用了以下技术：</p>\n"
            "            <ul style=\"margin-left: 1.5rem; margin-bottom: 2rem;\">\n"
            "                <li><code>socket()</code> - 创建网络套接字</li>\n"
            "                <li><code>bind()</code> - 绑定端口</li>\n"
            "                <li><code>listen()</code> - 监听连接</li>\n"
            "                <li><code>accept()</code> - 接受连接</li>\n"
            "                <li><code>pthread</code> - 多线程处理</li>\n"
            "                <li><code>open()/read()</code> - 文件操作</li>\n"
            "            </ul>\n"
            "            \n"
            "            <div class=\"footer\">\n"
            "                <p>🤖 服务器运行中 | 📅 %s | ⏰ 实时处理请求</p>\n"
            "                <p style=\"font-size: 0.9rem; margin-top: 0.5rem;\">\n"
            "                    这是一个教学示例，展示了如何用纯C语言实现完整的HTTP服务器。\n"
            "                </p>\n"
            "            </div>\n"
            "        </div>\n"
            "    </div>\n"
            "    \n"
            "    <script>\n"
            "        // 显示当前时间\n"
            "        function updateTime() {\n"
            "            const now = new Date();\n"
            "            const timeStr = now.toLocaleTimeString();\n"
            "            const timeElement = document.createElement('div');\n"
            "            timeElement.innerHTML = `<strong>客户端时间：</strong>${timeStr}`;\n"
            "            const footer = document.querySelector('.footer');\n"
            "            footer.insertBefore(timeElement, footer.firstChild);\n"
            "        }\n"
            "        \n"
            "        // 实时更新\n"
            "        updateTime();\n"
            "        setInterval(updateTime, 1000);\n"
            "        \n"
            "        // 按钮点击效果\n"
            "        document.querySelectorAll('.btn').forEach(btn => {\n"
            "            btn.addEventListener('click', function(e) {\n"
            "                if (this.getAttribute('href').startsWith('http')) {\n"
            "                    return; // 外部链接正常跳转\n"
            "                }\n"
            "                e.preventDefault();\n"
            "                this.style.transform = 'scale(0.95)';\n"
            "                setTimeout(() => {\n"
            "                    this.style.transform = '';\n"
            "                    window.location.href = this.getAttribute('href');\n"
            "                }, 150);\n"
            "            });\n"
            "        });\n"
            "    </script>\n"
            "</body>\n"
            "</html>\n", PORT, __DATE__);
        fclose(html);
    }
    
    // 创建示例CSS文件
    FILE* css = fopen("style.css", "w");
    if (css) {
        fprintf(css,
            "/* 示例CSS文件 */\n"
            "body {\n"
            "    background-color: #f0f0f0;\n"
            "    font-family: Arial, sans-serif;\n"
            "}\n"
            "\n"
            ".demo-box {\n"
            "    border: 2px solid #3498db;\n"
            "    padding: 20px;\n"
            "    border-radius: 10px;\n"
            "    background: white;\n"
            "}\n");
        fclose(css);
    }
    
    // 创建示例JS文件
    FILE* js = fopen("script.js", "w");
    if (js) {
        fprintf(js,
            "// 示例JavaScript文件\n"
            "console.log('C语言HTTP服务器正在运行！');\n"
            "\n"
            "// 添加点击计数器\n"
            "let clickCount = 0;\n"
            "document.addEventListener('click', function() {\n"
            "    clickCount++;\n"
            "    console.log('页面被点击了 ' + clickCount + ' 次');\n"
            "});\n");
        fclose(js);
    }
    
    // 创建静态目录示例文件
    FILE* demo = fopen("static/demo.txt", "w");
    if (demo) {
        fprintf(demo, "这是一个示例文本文件。\n");
        fprintf(demo, "你可以在这个文件夹中放置任何静态文件。\n");
        fprintf(demo, "服务器会自动提供这些文件。\n");
        fclose(demo);
    }
    
    FILE* readme = fopen("static/README.md", "w");
    if (readme) {
        fprintf(readme, "# 静态文件目录\n\n");
        fprintf(readme, "这个目录用于存放静态文件。\n");
        fprintf(readme, "服务器会自动提供这些文件。\n");
        fclose(readme);
    }
    
    printf("已创建示例文件:\n");
    printf("  • index.html - 主页\n");
    printf("  • style.css - 样式表示例\n");
    printf("  • script.js - JavaScript示例\n");
    printf("  • static/ - 静态文件夹\n");
    printf("  • static/demo.txt - 示例文本文件\n");
    printf("  • static/README.md - 说明文件\n");
    printf("================================================================\n");
    
    // 主循环：接受客户端连接
    while (1) {
        int* client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        
        if (*client_fd >= 0) {
            printf("客户端连接: %s:%d\n", 
                   inet_ntoa(client_addr.sin_addr), 
                   ntohs(client_addr.sin_port));
            
            pthread_t thread;
            pthread_create(&thread, NULL, client_thread, client_fd);
            pthread_detach(thread);
        } else {
            free(client_fd);
        }
    }
    
    close(server_fd);
    return 0;
}