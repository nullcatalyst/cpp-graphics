#import <Cocoa/Cocoa.h>

#import "./AppDelegate.h"

int main() {
    @autoreleasepool {
        NSApplication * application = [NSApplication sharedApplication];
        [application setActivationPolicy:NSApplicationActivationPolicyRegular];

        AppDelegate * appDelegate = [[AppDelegate alloc] init];
        [application setDelegate:appDelegate];
        [application activateIgnoringOtherApps:YES];
        [application run];
    }

    return EXIT_SUCCESS;
}
