import React, { FC, memo } from 'react';

import PlayBoard from '../containers/PlayBoard';
import PlayHelper from '../containers/PlayHelper';
import Share from '../components/Share';
import DigitBoard from '../containers/DigitBoard';
import HotKeys from '../containers/HotKeys';

const Game: FC = memo(({}) => {
    return (
        <>
            <HotKeys />
            <PlayBoard />
            <PlayHelper />
            <DigitBoard />
            <Share content={''} />
        </>
    );
});

export default Game;
