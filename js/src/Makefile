# make file for wpr_tracing.
# author: watson
# Date: Nov 24, 2013

RM := rm -rf

CXXFLAGS= -g -I./inc -I./src -fpic -DLINUX

LDFLAGS= -g -fPIC   

EX_OBJS=js.o

SO_OBJS=JsScriptScanner.o \
     JsScript.o \
     JsScriptNode.o \
     JsScriptNodeBinaryOperator.o \
     JsScriptNodeConstructorCall.o \
     JsScriptNodeFunction.o \
     JsScriptNodeFunctionCall.o \
     JsScriptNodeIdDotLiteral.o \
     JsScriptNodeIdentifier.o \
     JsScriptNodeLiteral.o \
     JsScriptNodeNoArgOperation.o \
     JsScriptNodeArrayInitialiser.o \
     JsScriptNodeObjectInitialiser.o \
     JsScriptNodeQuadrupleOperation.o \
     JsScriptNodeStatementBlock.o \
     JsScriptNodeSwitch.o \
     JsScriptNodeTernaryOperator.o \
     JsScriptNodeUnaryOperator.o \
     JsScriptNodeVarDeclaration.o \
     JsScriptNodeInbuiltFunctionCall.o \
     JsScannerUtilities.o \
     JsScannerException.o \
     JsScriptObject.o \
     JsScriptObjectActivation.o \
     JsScriptObjectArray.o \
     JsScriptObjectDate.o \
     JsScriptObjectMath.o \
     JsScriptObjectError.o \
     JsScriptObjectExternal.o \
     JsScriptObjectFunction.o \
     JsScriptObjectReferenceError.o \
     JsScriptObjectSystemCall.o \
     JsScriptObjectTypeError.o \
     JsScriptObjectUriError.o \
     JsScriptParser.o \
     JsScriptPredefinedNameTable.o \
     JsScriptPreprocessor.o \
     JsScriptProperty.o \
     JsScriptReference.o \
     JsScriptReferenceError.o \
     JsScriptStatistics.o \
     JsScriptString.o \
     JsScriptTraceContext.o \
     JsScriptTypeError.o \
     JsScriptUriError.o \
     JsScriptValue.o \
     JsScriptValueReader.o \
     JsTime.o \
     JsScriptExecutionTimeExceeded.o \
     JsScriptExecutionContext.o \
     JsScriptRuntime.o

BINS=libjs.so js
# project lifecycle target.
all: release

release: $(BINS) 

libjs.so: $(SO_OBJS)
	$(CXX) $(LDFLAGS) -shared -o $@ $(SO_OBJS) 

js: $(EX_OBJS)
	$(CXX) $(LDFLAGS) -L. -ljs -o $@ $(EX_OBJS)

# Other Targets
clean:
	-$(RM) $(SO_OBJS) $(EX_OBJS) $(BINS)
	-@echo ' '

.PHONY: all clean test
.SECONDARY:
