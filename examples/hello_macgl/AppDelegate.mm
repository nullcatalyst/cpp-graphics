#import "./AppDelegate.h"
#import "./GLView.h"

@implementation AppDelegate {
    NSString *  _appName;
    NSWindow *  _window;
}

- (nonnull instancetype) init {
    if ((self = [super init])) {
        _appName = [[NSProcessInfo processInfo] processName];

        NSApplication * application = [NSApplication sharedApplication];
        [application setMainMenu:[self createMenu]];
        _window = [self createWindow];
    }

    return self;
}

- (void) applicationWillFinishLaunching:(NSNotification *)notification {
    [_window setTitle:_appName];
    [_window cascadeTopLeftFromPoint:NSMakePoint(20, 20)];
    [_window makeKeyAndOrderFront:self];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return TRUE;
}

- (NSMenu *) createMenu {
    NSMenuItem * quit   = [[NSMenuItem alloc] initWithTitle:[@"Quit " stringByAppendingString:_appName] action:@selector(terminate:) keyEquivalent:@"q"];
    NSMenuItem * item   = [[NSMenuItem alloc] init];
    item.submenu        = [[NSMenu alloc] init];
    [item.submenu addItem:quit];

    NSMenu * mainMenu   = [[NSMenu alloc] init];
    [mainMenu addItem:item];

    return mainMenu;
}

- (NSWindow *) createWindow {
    NSWindow * window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 640, 480)
                                                    styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable
                                                      backing:NSBackingStoreBuffered
                                                        defer:NO];

    GLView * glView = [[GLView alloc] initWithFrame:window.contentView.frame];
    [window.contentView addSubview:glView];

    return window;
}

@end
