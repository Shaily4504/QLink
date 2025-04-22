import './App.css'
import {Route, Routes,HashRouter} from 'react-router-dom'
import { Login } from './Login/login'
import { Loadit } from './Upload/loadit'


const App = () => {

  return (
    <>
    <HashRouter>
    <Routes>
        <Route path='/' element={<Login/>}/>
        <Route path='/Loadit' element={<Loadit/>}/>
    </Routes>
    </HashRouter>
    </>
  )
}
export default App