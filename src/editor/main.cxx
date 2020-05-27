//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#include "application.hxx"
#include "mainWindow.hxx"
#include <cstdlib>

namespace pak
{
    static bool entryPoint(int argc, char* argv[])
    {
        pak::Application app(argc, argv);
        pak::MainWindow mainWindow;

        mainWindow.show();

        return app.exec() == 0;
    }
} // namespace pak

#if (defined _WIN32)
#include <Windows.h>
int WinMain(HINSTANCE, HINSTANCE, char*, int)
{
    return pak::entryPoint(__argc, __argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
#else
int main(int argc, char* argv[])
{
    return pak::entryPoint(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif // #if (defined _WIN32)
