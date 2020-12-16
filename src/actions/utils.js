function createActionFunc(type, ...argNames) {
    return function (...args) {
        let action = { type };
        argNames.forEach((argName, index) => {
            action[argNames[index]] = args[index];
        });
        return action;
    };
}

export default createActionFunc;
