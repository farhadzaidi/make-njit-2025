import React, { useState, useCallback, useEffect } from 'react';
import PropTypes from 'prop-types';

const GRID_SIZE = 20;
const CELL_SIZE = 30; // pixels

const PathGrid = () => {
  const [path, setPath] = useState([]);
  const [isDrawing, setIsDrawing] = useState(false);
  const [lastCell, setLastCell] = useState(null);
  const [hoverCell, setHoverCell] = useState(null);
  const [isUndoing, setIsUndoing] = useState(false);

  const resetPath = useCallback(() => {
    setPath([]);
    setIsDrawing(false);
    setLastCell(null);
    setHoverCell(null);
    setIsUndoing(false);
  }, []);

  const isValidMove = useCallback((currentCell, newCell) => {
    if (!currentCell) return true;

    const [currentRow, currentCol] = currentCell;
    const [newRow, newCol] = newCell;

    // Allow both orthogonal and diagonal moves
    const isAdjacent =
      Math.abs(currentRow - newRow) <= 1 &&
      Math.abs(currentCol - newCol) <= 1;

    // Check if cell is already in path
    const isAlreadyInPath = path.some(([row, col]) => row === newRow && col === newCol);

    // Allow moving to the previous cell (undo)
    const isPreviousCell = path.length > 1 &&
      newRow === path[path.length - 2][0] &&
      newCol === path[path.length - 2][1];

    return isAdjacent && (isPreviousCell || !isAlreadyInPath);
  }, [path]);

  const handleCellClick = useCallback((row, col) => {
    if (path.length === 0) {
      setPath([[row, col]]);
      setLastCell([row, col]);
      setIsDrawing(true);
    }
  }, [path]);

  const handleCellEnter = useCallback((row, col) => {
    if (!isDrawing || !lastCell) return;

    const newCell = [row, col];
    const isPreviousCell = path.length > 1 &&
      row === path[path.length - 2][0] &&
      col === path[path.length - 2][1];

    if (isValidMove(lastCell, newCell)) {
      if (isPreviousCell) {
        // Remove the last cell (undo)
        setPath(prevPath => prevPath.slice(0, -1));
        setLastCell(path[path.length - 2]);
        setIsUndoing(true);
      } else {
        setPath(prevPath => [...prevPath, newCell]);
        setLastCell(newCell);
        setIsUndoing(false);
      }
    }
  }, [isDrawing, lastCell, isValidMove, path]);

  const handleMouseUp = useCallback(() => {
    if (path.length > 0) {
      console.log('Path coordinates:', path);
    }
    setIsDrawing(false);
    setHoverCell(null);
    setIsUndoing(false);
  }, [path]);

  useEffect(() => {
    window.addEventListener('mouseup', handleMouseUp);
    return () => window.removeEventListener('mouseup', handleMouseUp);
  }, [handleMouseUp]);

  const getCellStyle = useCallback((row, col) => {
    const isInPath = path.some(([r, c]) => r === row && c === col);
    const isHovered = hoverCell && hoverCell[0] === row && hoverCell[1] === col;

    if (!isInPath && !isHovered) return {};

    const isStart = row === path[0]?.[0] && col === path[0]?.[1];
    const isEnd = row === path[path.length - 1]?.[0] && col === path[path.length - 1]?.[1];
    const isCorner = path.length > 2 && path.slice(1, -1).some(([r, c]) => r === row && c === col);
    const isPreviousCell = path.length > 1 &&
      row === path[path.length - 2][0] &&
      col === path[path.length - 2][1];

    return {
      backgroundColor: isStart ? '#4CAF50' : // green
        isEnd ? '#f44336' : // red
          isCorner ? '#2196F3' : // blue
            isHovered && isPreviousCell ? '#FFA726' : // orange for undo hover
              isHovered ? '#E3F2FD' : // light blue for hover
                '#90CAF9', // light blue for path
      transform: isHovered ? 'scale(1.1)' : 'scale(1)',
      transition: 'all 0.15s ease',
      zIndex: isHovered ? 1 : 0,
      border: isHovered && isPreviousCell ? '2px solid #FF9800' : undefined,
    };
  }, [path, hoverCell]);

  return (
    <div className="path-grid-container">
      <div
        className="path-grid"
        style={{
          display: 'grid',
          gridTemplateColumns: `repeat(${GRID_SIZE}, ${CELL_SIZE}px)`,
          gap: '1px',
          padding: '20px',
          backgroundColor: '#e0e0e0',
          width: 'fit-content',
          borderRadius: '8px',
          boxShadow: '0 2px 4px rgba(0,0,0,0.1)',
        }}
      >
        {Array.from({ length: GRID_SIZE }, (_, row) =>
          Array.from({ length: GRID_SIZE }, (_, col) => (
            <div
              key={`${row}-${col}`}
              className="grid-cell"
              style={{
                width: CELL_SIZE,
                height: CELL_SIZE,
                backgroundColor: 'white',
                border: '1px solid #ccc',
                cursor: 'pointer',
                borderRadius: '4px',
                ...getCellStyle(row, col),
              }}
              onClick={() => handleCellClick(row, col)}
              onMouseEnter={() => {
                setHoverCell([row, col]);
                handleCellEnter(row, col);
              }}
            />
          ))
        )}
      </div>
      <button
        onClick={resetPath}
        style={{
          marginTop: '20px',
          padding: '10px 20px',
          backgroundColor: '#2196F3',
          color: 'white',
          border: 'none',
          borderRadius: '4px',
          cursor: 'pointer',
          fontSize: '16px',
          transition: 'background-color 0.2s ease',
          boxShadow: '0 2px 4px rgba(0,0,0,0.1)',
        }}
        onMouseEnter={(e) => e.target.style.backgroundColor = '#1976D2'}
        onMouseLeave={(e) => e.target.style.backgroundColor = '#2196F3'}
      >
        Reset Path
      </button>
    </div>
  );
};

PathGrid.propTypes = {};

export default PathGrid; 