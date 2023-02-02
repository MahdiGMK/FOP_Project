#include <shared/CommandReader.h>
#include <shared/FileHandling.h>

int main()
{
    while (1)
    {
        fclose(CreateOStream());
        ReadCMD(stdin);
        PrintOStream();
    }
}
