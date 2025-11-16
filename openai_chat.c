#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


// Structure to store response data
struct Response {
    char *data;
    size_t size;
};


// Callback function to write response data
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, struct Response *response) {
    size_t realsize = size * nmemb;
    char *ptr = realloc(response->data, response->size + realsize + 1);
    
    if (ptr == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }
    
    response->data = ptr;
    memcpy(&(response->data[response->size]), contents, realsize);
    response->size += realsize;
    response->data[response->size] = 0; // null terminate
    
    return realsize;
}

int main() {
    CURL *curl;
    CURLcode res;
    struct Response response = {0};
    struct curl_slist *headers = NULL;
    
    // Initialize libcurl
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_global_init() failed: %s\n", curl_easy_strerror(res));
        return 1;
    }
    
    curl = curl_easy_init();
    if (curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        
        // Set headers
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        // Try to get API key from file first, then fall back to environment variable
        char api_key_buffer[512] = {0};
        const char *api_key = NULL;
        
        // Try to read API key from file
        FILE *key_file = fopen("api_key.txt", "r");
        if (key_file != NULL) {
            if (fgets(api_key_buffer, sizeof(api_key_buffer), key_file) != NULL) {
                // Remove newline character if present
                char *newline = strchr(api_key_buffer, '\n');
                if (newline) *newline = '\0';
                // Remove carriage return if present
                char *carriage_return = strchr(api_key_buffer, '\r');
                if (carriage_return) *carriage_return = '\0';
                
                api_key = api_key_buffer;
                printf("✓ API key loaded from api_key.txt\n");
            }
            fclose(key_file);
        }
        
        // Fall back to environment variable if file reading failed
        if (api_key == NULL || strlen(api_key) == 0) {
            api_key = getenv("OPENAI_API_KEY");
            if (api_key != NULL) {
                printf("✓ API key loaded from environment variable\n");
            }
        }
        
        // Check if we have an API key
        if (api_key == NULL || strlen(api_key) == 0) {
            fprintf(stderr, "Error: No API key found!\n");
            fprintf(stderr, "Options:\n");
            fprintf(stderr, "1. Create api_key.txt file with your API key\n");
            fprintf(stderr, "2. Set OPENAI_API_KEY environment variable\n");
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return 1;
        }
        
        // Create authorization header
        char auth_header[512];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
        headers = curl_slist_append(headers, auth_header);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        // JSON payload with "hello" message
        const char *json_data = 
            "{"
            "\"model\": \"gpt-3.5-turbo\","
            "\"messages\": ["
                "{"
                "\"role\": \"user\","
                "\"content\": \"hello\""
                "}"
            "],"
            "\"max_tokens\": 100"
            "}";
        
        // Set POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        
        // Set callback function to capture response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        // Perform the request
        printf("Sending request to OpenAI API with message: \"hello\"\n");
        printf("Request payload: %s\n\n", json_data);
        
        res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Response from OpenAI API:\n");
            printf("%s\n", response.data ? response.data : "No response data");
        }
        
        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    
    // Free response data
    if (response.data) {
        free(response.data);
    }
    
    curl_global_cleanup();
    return (res == CURLE_OK) ? 0 : 1;
}