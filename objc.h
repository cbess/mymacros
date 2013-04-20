// Helpful Obj-C/C++ macros

// Expands to a value in MBs
#define SizeMB(EXPR) 1024 * 1024 * EXPR

#pragma mark - Debug

#ifdef DEBUG
#   define CBDLog NSLog
#   define CBDebugLog(MSG, ...) NSLog((@"%s:%d "MSG), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#   define CBDebugMark() CBDebugLog(@"called");
    // outputs the specified code block (can be multi-line)
#   define CBDebugCode(BLOCK) BLOCK
#else
#   define CBDLog(X, ...) ;
#   define CBDebugLog ;
#   define CBDebugMark() ;
#   define CBDebugCode(BLOCK) ;
#endif

#pragma mark - ARC Support

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

#define NSRelease(OBJ) NO_ARC([OBJ release]); OBJ = nil;
#define NSAutoRelease(OBJ) IF_ARC(OBJ, [OBJ autorelease]);
#define NSRetain(OBJ) IF_ARC(OBJ, [OBJ retain]);

// outputs a log that is more readable during unit test output
#define TLog(X, ...) DLog(@">>"); DLog(X, ##__VA_ARGS__); DLog(@"<<");

#pragma mark - NS Utility Methods

// NSURL with string
static inline NSURL * URL(NSString *urlString)
{
    return [NSURL URLWithString:urlString];
}

// NSNumber with int shortcut
static inline NSNumber * NumberInt(int value)
{
    return [NSNumber numberWithInt:value];
}

// NSNumber with double shortcut
static inline NSNumber * NumberDouble(double value)
{
    return [NSNumber numberWithDouble:value];
}

// NSNumber with float shortcut
static inline NSNumber * NumberFloat(float value)
{
    return [NSNumber numberWithFloat:value];
}

// alias for [NSString stringWithFormat:format, ...]
static NSString * nssprintf(NSString *format, ...)
{
    va_list args;
    va_start(args, format);
    NSString *string = [[NSString alloc] initWithFormat:format 
                                              arguments:args];
    va_end(args);
    
	return NSAutoRelease(string);
}

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

// Specifies the type of device executing the code (running the app)
typedef enum {
    DeviceTypePad,
    DeviceTypePhone
} DeviceType;

// Determines the type of device this application is running on (4.0+)
static BOOL IsDevice(DeviceType dType)
{
    switch (dType)
    {
        case DeviceTypePad:
            return ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad);
            
        case DeviceTypePhone:
            return ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone);
    }
    
    return NO;
}

#define IsPhone() IsDevice(DeviceTypePhone)
#define IsPad() IsDevice(DeviceTypePad)

// if 'is iPad' macro
#define IF_PAD(T_BLOCK, F_BLOCK) (IsPad() ? T_BLOCK : F_BLOCK)

// Expands to an expression that evals to YES, if the current application is active
#define IsAppActive() [UIApplication sharedApplication].applicationState == UIApplicationStateActive

// Expands to load IB method call, returning the first object in the nib
#define IB_OBJECT(NAME) [[[NSBundle mainBundle] loadNibNamed:NAME owner:self options:NULL] objectAtIndex:0]
    
// Expands to load IB for device
#define DEVICE_IB_OBJECT(NAME) IB_OBJECT(DeviceNibName(NAME))
    
// Expands to load IB call using the current class name, adds ipad suffix when needed
// Can be used in [init] or [loadView]
#define DEVICE_IB_SELF() IB_OBJECT(DeviceNibName(NSStringFromClass([self class])))
    
// Expands to load IB method call, using current class name for nib name
#define IB_SELF() IB_OBJECT(NSStringFromClass([self class]));
    
// Expands to standard nib loading logic when initializing a UIView
#define IB_INIT_NAME(NAME) ({ \
    UIView *__view = IB_OBJECT(NAME); \
    self.frame = __view.bounds; \
    self.autoresizingMask = __view.autoresizingMask; \
    [self addSubview:__view]; });

// Expands to same as above, except it places the first view in the contentView
#define IB_INIT_CELL() ({ \
    UIView *view = IB_SELF() \
    [self.contentView addSubview:view]; });
    
// Expands to same as above, except it uses the class name
#define IB_INIT() IB_INIT_NAME(NSStringFromClass([self class]))
    
// Expands to same as above, except add the device suffix
#define DEVICE_IB_INIT() IB_INIT_NAME(DeviceNibName(NSStringFromClass([self class])))
    
// Expands to an initWithFrame:CGRectZero call, this is mostly for views that use IB_* macros
#define InitView(CLASS) [[CLASS alloc] initWithFrame:CGRectZero];
    
// Sets the view's background to the given image
static inline void SetBackgroundImage(UIView *view, NSString *imageName)
{
    view.layer.contents = (id)[UIImage imageNamed:imageName].CGImage;
}

// Returns the image name for the current iOS device, appends 'png'
static inline NSString * DeviceImageName(NSString *imageName)
{
    return [imageName stringByAppendingString:(IsPhone() ? @".png" : @"~ipad.png")];
}

// Returns the xib/nib name for the current iOS device
static inline NSString * DeviceNibName(NSString *nibName)
{
    return (IsPhone() ? nibName : [nibName stringByAppendingString:@"_iPad"]);
}

// creates a boolean statment that yeilds YES, if the table is the view controller search results table view
#define IsSearchResultsTableView(TABLE) \
		(TABLE == self.searchDisplayController.searchResultsTableView)

/**
 * Creates a UIView core animation block
 */
#define BEGIN_ANIMATE \
    [UIView beginAnimations:nil \
    context:NULL];

#define BEGIN_ANIMATE_CONTEXT(CTX) \
    [UIView beginAnimations:nil \
    context:CTX];

#define BEGIN_ANIMATE_DURATION(DUR) \
    BEGIN_ANIMATE \
    [UIView setAnimationDuration:DUR];

#define END_ANIMATE \
    [UIView commitAnimations];

// Expands to animate block
#define AnimateUI(DURATION, BLOCK) \
    [UIView animateWithDuration:DURATION animations:^{ BLOCK; }];

// Expands to the animate block with a default duration
#define AnimateUI1(BLOCK) AnimateUI(0.3, BLOCK)

// animates the contents of the animations block, executing the completed block after the animations
static void animate_block(NSTimeInterval duration, void (^animationsBlock)(void), void (^completedBlock)(BOOL done))
{
    if (duration == 0)
    {
        if (animationsBlock)
            animationsBlock();
        
        if (completedBlock)
            completedBlock(YES);
    }
    else
    {
        [UIView animateWithDuration:duration
                         animations:animationsBlock
                         completion:completedBlock];
    }
}

// enable/disabe user interaction (touches)
#define UI_ENABLE_TOUCHES [[UIApplication sharedApplication] endIgnoringInteractionEvents];
#define UI_DISABLE_TOUCHES [[UIApplication sharedApplication] beginIgnoringInteractionEvents];

// Sets the view's background to the given image
static inline void SetBackgroundImage(UIView *view, NSString *imageName) 
{
    view.layer.contents = (id)[UIImage imageNamed:imageName].CGImage;
}

/**
 * Modify UIView frame macros
 */
static void UIViewSetOrigin(UIView *view, CGFloat x, CGFloat y)
{
    CGRect rect = view.frame;
    
    if (x >= 0)
        rect.origin.x = x;
    if (y >= 0)
        rect.origin.y = y;
    
    view.frame = rect;
}

// sets the size of the frame for the specified view
static void UIViewSetSize(UIView *view, CGFloat width, CGFloat height)
{
    CGRect rect = view.frame;
    
    if (width >= 0)
        rect.size.width = width;
    if (height >= 0)
        rect.size.height = height;
    
    view.frame = rect;
}

// UIColor from RGB
#define UIColorRGB(R, G, B) [UIColor colorWithRed:R green:G blue:B alpha:1.0];

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
