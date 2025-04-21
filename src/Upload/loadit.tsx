import React, { useState } from 'react';
import { ToastContainer, toast } from 'react-toastify';
import '@coreui/coreui-pro/dist/css/coreui.min.css';
import 'react-toastify/dist/ReactToastify.css';
// import { useNavigate } from 'react-router-dom';
import Select from 'react-select';
import axios from 'axios';

type OptionType = {
  value: string;
  label: string;
};

export const Loadit = () => {
  // const navigate = useNavigate();

  const [file, setFile] = useState<File | null>(null);
  const [fileName, setFileName] = useState('');
  const [selectedOption, setSelectedOption] = useState<OptionType | null>(null);

  const options: OptionType[] = [
    { value: 'yashpandey', label: 'Yash Bhushan Pandey' },
    { value: 'altamashbeg', label: 'Altamash Beg' },
    { value: 'priyanshkumarrai', label: 'Priyansh Kumar Rai' },
  ];

  const handleFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const selected = e.target.files?.[0];
    if (selected) {
      setFile(selected);
      setFileName(selected.name);
    }
  };

  const handleSubmit = async (e: React.FormEvent<HTMLFormElement>) => {
    e.preventDefault();
  
    if (!file) {
      toast.error('No file selected');
      return;
    }
  
    if (!selectedOption) {
      toast.error('Please select a sender');
      return;
    }
  
    try {
      const formData = new FormData();
      formData.append('file', file);
      formData.append('receiver', selectedOption.value); // ✅ This line sends the selected user
  
      const response = await axios.post('http://localhost:5200/upload-files', formData, {
        headers: { 'Content-Type': 'multipart/form-data' },
      });
  
      if (!response.data?.pdf?.fileUrl) {
        toast.error('File upload failed: Missing fileUrl');
        return;
      }
  
      toast.success('Uploaded Successfully!', {
      });
    } catch (error) {
      console.error('Error submitting form:', error);
      toast.error('Submission failed, please try again');
    }
  };
  

  return (
    <div className="fixed inset-0 flex flex-col items-center justify-center bg-gradient-to-r from-[#3c50e0] to-[#00df9a] text-white px-6">
      <ToastContainer />
      <form onSubmit={handleSubmit} className="flex flex-col items-center w-full max-w-xl space-y-6">
        <h1 className="text-4xl font-bold font-Manrope text-center">
          Upload Important Files and Select the Sender
        </h1>

        {/* File Select Button */}
        <div className="flex flex-col items-center gap-3">
          <label htmlFor="file-upload" className="cursor-pointer px-6 py-2 bg-black rounded-xl font-semibold hover:bg-white hover:text-black transition">
            Select File
          </label>
          <input
            type="file"
            id="file-upload"
            className="hidden"
            onChange={handleFileChange}
          />
          {fileName ? <p className="text-white">📄 {fileName}</p> : <p className="text-white font-bold">No file selected</p>}
        </div>

        {/* Sender Dropdown */}
        <div className="w-full">
          <Select
            placeholder="Select Sender"
            value={selectedOption}
            onChange={(option) => setSelectedOption(option as OptionType)}
            options={options}
            className="text-black"
            styles={{
              control: (base) => ({
                ...base,
                borderRadius: '10px',
                padding: '2px',
                fontSize: '16px',
              }),
            }}
          />
        </div>

        {/* Submit */}
        <button
          type="submit"
          className="px-8 py-2 bg-black text-white font-bold rounded-full hover:bg-white hover:text-black transition"
        >
          Upload Now
        </button>
        <img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAKQAAACkCAYAAAAZtYVBAAAAAklEQVR4AewaftIAAAYhSURBVO3BQY4cSRLAQDLQ//8yV0c/JZCoak2s4Gb2B2td4rDWRQ5rXeSw1kUOa13ksNZFDmtd5LDWRQ5rXeSw1kUOa13ksNZFDmtd5LDWRQ5rXeSw1kV++JDK31TxRGWqmFSeVEwqU8UbKk8qJpWpYlKZKp6o/E0VnzisdZHDWhc5rHWRH76s4ptUPqHypOJJxaQyVbxR8YmKSWWqeFLxTSrfdFjrIoe1LnJY6yI//DKVNyreUHlSMam8ofKJijcqJpWp4ptU3qj4TYe1LnJY6yKHtS7ywz9OZaqYVKaKqWJSeUNlqniiMlVMKv+yw1oXOax1kcNaF/nhH1PxRsUTlScqTyqeqEwVk8pU8S87rHWRw1oXOax1kR9+WcXfpDJVPFGZKp5UPFF5ojJVvKEyVXyi4iaHtS5yWOsih7Uu8sOXqfyXKiaVqeKbVKaKSWWqmFSmiicVk8pU8UTlZoe1LnJY6yKHtS7yw4cqblbxhspU8YmKSeWJyjdV/D85rHWRw1oXOax1kR8+pDJVTCrfVDFVTCpTxaTym1SmijcqJpVJ5Q2Vb6r4TYe1LnJY6yKHtS7yw4cqJpWp4onKVPGGyhsVk8obKlPFpDKpPKmYVN6oeKPiicp/6bDWRQ5rXeSw1kXsDy6mMlU8UblJxRsqU8UTlaliUpkqJpWpYlJ5UvFNh7UucljrIoe1LmJ/8BepvFHxROVJxRsqU8UTlTcq3lCZKp6oTBVPVN6o+E2HtS5yWOsih7Uu8sOHVD5R8URlqnhS8UTlScWkMlU8qXhD5UnFpDJVTBWTypOKSWWqmFSeVHzisNZFDmtd5LDWRewPvkhlqvibVJ5UTCpPKiaVqeKJylTxROW/VDGpvFHxicNaFzmsdZHDWhexP/iAylQxqTypeKIyVUwqU8UbKr+p4onKVPFE5RMVn1CZKr7psNZFDmtd5LDWRewPvkhlqniiMlV8QuWNikllqphUpoonKlPFE5UnFZ9QmSomlScVv+mw1kUOa13ksNZFfvjLVKaKJypvVDxR+U0qU8UTlaliUnmi8qTiiconVKaKTxzWushhrYsc1rrID39ZxRsVb6hMFU9UpopJZap4UjGpPKl4Q2WqeKLypGJSeaLymw5rXeSw1kUOa13khw+pTBVPVJ5UTCrfVDGpTCpTxaTypOJJxROVqeINlaniicpUMak8qfimw1oXOax1kcNaF/nhy1Smik9UTCpTxaQyqUwVb6hMFW9UTCpTxSdUvknlDZWp4hOHtS5yWOsih7Uu8sOHKj5RMak8qXhS8YmKSWVSmSqeqEwVf5PKVDFVTCpTxd90WOsih7UucljrIj98SGWqmFTeqPiEypOKqeITKk8qnqh8ouINlScVb6hMFZ84rHWRw1oXOax1kR/+YypvVDypeKLyiYo3VJ5UTCqTyjdVPFF5o+KbDmtd5LDWRQ5rXcT+4AMqU8UTlaliUpkqnqhMFW+oPKl4ojJVPFF5o+INld9U8ZsOa13ksNZFDmtd5Ie/rGJSmSomlaniicobFd+kMlW8UfGGyt+k8qTiE4e1LnJY6yKHtS7yw4cqPlHxpOKNipuoPKl4ojJVPKl4Q2WqmFSmit90WOsih7UucljrIj98SOVvqpgqJpVvqphU3qh4ojJVPFF5Q2Wq+CaVqeITh7UucljrIoe1LvLDl1V8k8oTlScVn1D5hMpU8UbFpPJGxf+Tw1oXOax1kcNaF/nhl6m8UfFNKk8qvqniEypPKiaVSeUTKlPFpPKbDmtd5LDWRQ5rXeSHf0zFpPJEZap4o2JSeVIxVbyhMlVMKlPFpPKkYlJ5UvFNh7UucljrIoe1LvLDP67iicqk8obKVPGGylTxpGJSmSomlScVb1T8psNaFzmsdZHDWhf54ZdV/KaKSeVJxRsVk8pU8URlqpgqJpWpYlKZKiaVN1Smiv/SYa2LHNa6yGGti/zwZSp/k8pUMak8qZhUJpU3VKaKJypTxZOKJxVPVN6omFSmim86rHWRw1oXOax1EfuDtS5xWOsih7UucljrIoe1LnJY6yKHtS5yWOsih7UucljrIoe1LnJY6yKHtS5yWOsih7UucljrIv8DrD4JSyLtt1MAAAAASUVORK5CYII=" />
      </form>
    </div>
  );
};

export default Loadit;
