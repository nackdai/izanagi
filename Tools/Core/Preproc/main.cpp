extern "C"
int preproc_main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    int ret = preproc_main(argc, argv);
    return ret;
}

