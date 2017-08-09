#ifndef PLUGINPRIVATE_H
#define PLUGINPRIVATE_H
static const struct {
    const char *type;
    int major, minor;
} qmldirprivate [] = {
    { "AppPage", 1, 0 },
    { "ConfigPage", 1, 0 },
    { "ErrorPage", 1, 0 },
    { "InstancePage", 1, 0 },
    { "LauncherPage", 1, 0 },
    { "SelectedPage", 1, 0 }
};
#endif // PLUGINPRIVATE_H

