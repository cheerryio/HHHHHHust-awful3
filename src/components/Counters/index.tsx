import React, { FC, memo } from 'react';
import { Props } from '../../containers/Counters';
import Button from '@material-ui/core/Button';

export const Counters: FC<Props> = memo((props) => {
    const { value, serverValue, incrementAction, decrementAction, getserverValueAction } = props;
    return (
        <div>
            {`Clicked:`}
            <strong>{value}</strong>
            {` times.`}
            <button onClick={incrementAction} style={{ marginRight: 20 }}>
                {' '}
                +{' '}
            </button>
            <button onClick={decrementAction}> - </button>
            <br />
            <span>{`serverValue is:`}</span>
            <strong>{serverValue}</strong>
            <Button variant='contained' color='primary' onClick={() => getserverValueAction('a')}>
                Change
            </Button>
        </div>
    );
});
