//
// Created by giacomo on 19/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_COPYABLESTACK_H
#define FUZZYSTRINGMATCHING2_COPYABLESTACK_H

template<class Type>
class CopyableStack
{
public:
    CopyableStack() : mStack() {}
    CopyableStack(const CopyableStack& x) {
        mStack = x.mStack;
    };
    CopyableStack& operator=(const CopyableStack& x) {
        mStack = x.mStack;
        return *this;
    };

    // Push onto the top of the stack
    Type push(const Type& val) {
        mStack.push_back(std::move(val));
    }

    // Get the element at the top
    Type& top() {
        return mStack.back();
    }

    // Get the element at the top
    const Type& top() const {
        return mStack.back();
    }

    // Remove the element at the top
    void pop() noexcept {
        mStack.pop_back();
    }

    bool empty() const {
        return mStack.empty();
    }

private:
    std::vector<Type> mStack;
};

#endif //FUZZYSTRINGMATCHING2_COPYABLESTACK_H
