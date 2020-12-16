import React, { FC, memo } from 'react';
import Button from '@material-ui/core/Button';

export interface Props {
    onClick: () => void;
}
const customDialog: FC<Props> = memo(() => {
    return (
        <Button variant='contained' color='primary'>
            Button
        </Button>
    );
});

export default customDialog;
