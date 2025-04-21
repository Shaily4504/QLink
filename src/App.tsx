import './App.css'
import { Route, Routes, BrowserRouter } from 'react-router-dom'
import { Login } from './Login/login'
import { Loadit } from './Upload/loadit'


const App = () => {

  return (
    <>
    <BrowserRouter>
    <Routes>
        <Route path='/' element={<Login/>}/>
        <Route path='/Loadit' element={<Loadit/>}/>
    </Routes>
    </BrowserRouter>
    </>
  )
}
export default App