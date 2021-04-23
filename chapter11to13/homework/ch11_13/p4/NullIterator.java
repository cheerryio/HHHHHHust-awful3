package homework.ch11_13.p4;

/**
 * 空迭代器，这个迭代器的 hasNext()方法永远返回 false, next()方法永远返回 null. 用于
 * AtomicComponent，因为 AtomicComponent 没有子组件. 因此 AtomicComponent 的 iterator 方法
 * 应该返回 NullIterator 的实例.
 */
public class NullIterator implements Iterator {
    /**
     * 是否还有元素
     *
     * @return 如果元素还没有迭代完，返回 true;否则返回 false
     */
    @Override
    public boolean hasNext() {
        return false;
    }

    /**
     * 获取下一个组件
     *
     * @return 下一个组件
     */
    @Override
    public Component next() {
        return null;
    }
}
