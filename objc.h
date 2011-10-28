// Helpful Obj-C/C++ macros

#ifdef DEBUG
#define DLog NSLog
#define DebugLog(MSG, ...) NSLog((@"%s:%d "MSG), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#define DebugMark() DebugLog(@"called");
// outputs the specified code block (can be multi-line)
#define DebugCode(BLOCK) BLOCK
#else
#define DLog(X, ...) ;
#define DebugLog ;
#define DebugMark() ;
#define DebugCode(BLOCK) ;
#endif

#define HAS_ARC __has_feature(objc_arc)

#if HAS_ARC
#define STRONGRETAIN strong
#define WEAKASSIGN weak
#define NO_ARC(BLOCK_NO_ARC) ;
#define IF_ARC(BLOCK_ARC, BLOCK_NO_ARC) BLOCK_ARC
#else
#define STRONGRETAIN retain
#define WEAKASSIGN assign
#define NO_ARC(BLOCK_NO_ARC) BLOCK_NO_ARC
#define IF_ARC(BLOCK_ARC, BLOCK_NO_ARC) BLOCK_NO_ARC
#endif

#define NSRelease(OBJ) NO_ARC([OBJ release]);

// outputs a log that is more readable during unit test output
#define TLog(X, ...) DLog(@">>"); DLog(X, ##__VA_ARGS__); DLog(@"<<");

// NSNumber with int shortcut
#define NumberInt(N) [NSNumber numberWithInt:N]
#define NumberFloat(N) [NSNumber numberWithFloat:N]

// NSString format shortcut
#define FormatString(S, ...) [NSString stringWithFormat:S, __VA_ARGS__]
#define FormatLocalizedString(S, ...) [NSString stringWithFormat:NSLocalizedString(S, nil), __VA_ARGS__]

/**
 * Adds a message to the class that adds support for the target device orientation
 * @params INTF: possible values = Portrait | Landscape
 */
#define SUPPORTS_DEVICE_ORIENTATION(INTF) \
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation \
{ return UIInterfaceOrientationIs##INTF(interfaceOrientation); }

/**
 * Adds a message to the class that adds support for the all device orientations
 */
#define SUPPORTS_ALL_DEVICE_ORIENTATIONS() \
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation \
{ return YES; }

typedef enum {
    DeviceTypePad,
    DeviceTypePhone
} DeviceType;

// Determines the type of device this application is running on
static BOOL IsDevice(DeviceType dType)
{
    switch (dType)
    {
        case DeviceTypePad:
            return (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad);
            
        case DeviceTypePhone:
            return (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone);
    }
    
    return NO;
}

#define IsPhone() IsDevice(DeviceTypePhone)
#define IsPad() IsDevice(DeviceTypePad)

/**
 * Expands to NSBundle nib named. Returns the first object from the nib.
 */
#define IB_OBJECT(NAME) \
    [[[NSBundle mainBundle] loadNibNamed:NAME owner:self options:nil] objectAtIndex:0];
    
// Expands to load nib call, using self as the owner and the class name as the nib name
#define IB_SELF() \
	IB_OBJECT(NSStringFromClass([self class]))

// Expands to standard nib loading logic when initializing a UIView
#define IB_INIT() ({ \
UIView *__view = IB_SELF() \
self.frame = __view.bounds; \
[self addSubview:__view]; });

// creates a boolean statment that yeilds YES, if the table is the view controller search results table view
#define IsSearchResultsTableView(TABLE) \
		(TABLE == self.searchDisplayController.searchResultsTableView)

/**
 * Creates a UIView core animation block
 */
#define BEGIN_ANIMATE_CONTEXT(CTX) \
[UIView beginAnimations:nil \
context:CTX];

#define BEGIN_ANIMATE \
[UIView beginAnimations:nil \
context:NULL];

#define END_ANIMATE \
[UIView commitAnimations];

// Expands to animate block
#define AnimateUI(DURATION, BLOCK) \
    [UIView animateWithDuration:DURATION animations:^{ BLOCK; }];

// Expands to the animate block with a default duration
#define AnimateUI1(BLOCK) AnimateUI(0.3, BLOCK)

// Expands to a safe delegate selector call
#define TryDelegateSelector(SELECTOR, ARG1) \
    if ([self.delegate respondsToSelector:@selector(SELECTOR:)]) \
        [self.delegate SELECTOR:ARG1];

#pragma mark - Thread Macros

#define ASYNC_START_QUEUE(LVL) \
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_##LVL, 0), ^{

#define MAIN_QUEUE_START \
dispatch_async(dispatch_get_main_queue(), ^{

#define END_ASYNC });