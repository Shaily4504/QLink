import React, { useRef } from 'react';

interface TestProps {
  handleFile: (file: File) => void;
}

export const Drag: React.FC<TestProps> = ({ handleFile }) => {
  const hiddenFileInput = useRef<HTMLInputElement>(null);

  const handleClick = (event: React.MouseEvent<HTMLButtonElement>) => {
    hiddenFileInput.current?.click();
  };

  const handleChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    if (event.target.files && event.target.files.length > 0) {
      const fileUploaded = event.target.files[0];
      handleFile(fileUploaded);
    }
  };

  return (
    <div className='flex flex-col items-center justify-center'>
      <button 
        className='bg-[#2d2d2c] text-center justify-center items-center mt-20 rounded-[12px] h-20 mb-5 w-80' 
        type="button"
        onClick={handleClick}
      >
        <p className='text-white text-2xl text-center font-bold font-Manrope'>Select The File</p>
      </button>
      <input 
        type='file' 
        name="fileUpload" 
        accept='application/pdf' 
        required 
        ref={hiddenFileInput} 
        onChange={handleChange}
        style={{ visibility: 'visible', height: 0 }} 
      />
    </div>
  );
};

export default Drag;
