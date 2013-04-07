#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include "mojoshader.h"
#include "MojoShaderWrapper.h"

#define VRETURN(cond)   if (!(cond)) { assert(false); return false; }
#define VGOTO(cond, label)  if (!(cond)) { assert(false); goto label; }

namespace izanagi {
namespace tool {
    bool ParseToGLSL(const char* filename, const char* output)
    {
        bool result = false;

        FILE* in = NULL;
        FILE* out = NULL;
        const MOJOSHADER_parseData* pd = NULL;

        ::fopen_s(&in, filename, "rt");
        result = (in != NULL);
        VGOTO(result, __EXIT__);

        ::fseek(in, 0, SEEK_END);
        long size = ::ftell(in);

        ::fseek(in, 0, SEEK_SET);

        unsigned char* buffer = (unsigned char*)::malloc(size);
        result = (buffer != NULL);
        VGOTO(result, __EXIT__);
        
        fread(buffer, sizeof(unsigned char), size, in);

        pd = MOJOSHADER_parse(
            "glsl",
            buffer,
            size,
            NULL, 0,
            NULL, 0,
            NULL, NULL,
            NULL);

        result = (pd->error_count == 0);

        if (result) {
            ::fopen_s(&out, output, "wb");
            result = (out != NULL);
            VGOTO(result, __EXIT__);

            fwrite(pd->output, sizeof(unsigned char), pd->output_len, out);
        }

    __EXIT__:
        if (in != NULL) {
            ::fclose(in);
        }
        if (out != NULL) {
            ::fclose(out);
        }
        if (buffer != NULL) {
            free(buffer);
        }
        if (pd != NULL) {
            MOJOSHADER_freeParseData(pd);
        }

        return result;
    }
}   // namespace izanagi
}   // namespace tool
