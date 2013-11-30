typedef struct Profile {
    char* ip;
    char* transport;
    int iopub_port;
    int shell_port;
    int control_port;
    int stdin_port;
    int hb_port;
    char* key;
    char* signature_scheme;
} Profile;

void init_profile(Profile* profile, const char* existing);
void free_profile(Profile* profile);
