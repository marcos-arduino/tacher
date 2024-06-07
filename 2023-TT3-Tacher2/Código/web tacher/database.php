<?php
function conexion() // definimos la funcion que va a devolver el objeto para realizar consultas
{
    try {
        $conexion = new PDO("mysql:host=localhost;port=3306;dbname=id21247720_tacher_db", "root", "");
        // este es el caso en que la conexion se esté haciendo para xampp porque requiere otros parametros
    } catch (PDOException) {
        $conexion = new PDO("mysql:host=sql106.byetcluster.com;port=3306;dbname=if0_35180481_tacher2", "if0_35180481", "o1nd3sUOIdKM6a");
        // esta conexion se realizará cuando se esté hosteando para 'www.tacher2.000.pe' en infinityfree
    } finally {
        $conexion->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        $conexion->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING);
        return $conexion; // se retorna el objeto para poder usarlo en las diferentes funciones que ejecutan sentencias sql
    }
}
function registro($dni, $nombre, $conexion) // se define la funcion capaz de registrar usuarios a la db
{
    try {
        $pdo = $conexion->prepare('INSERT INTO usuarios(dni, Nombre) VALUES(?, ?)'); // sentencia sql para insertar registros a la tabla usuarios
        $pdo->bindParam(1, $dni); // al primer parametro (?) le pasamos el valor de la variable $dni
        $pdo->bindParam(2, $nombre); // al segundo parametro (?) le pasamos el valor de la variable $nombre
        if ($pdo->execute()) {  // se compara la ejecucion de la sentencia sql
            header("Location: register.php?rgt=1"); // si se inserta correctamente se redirecciona a la pagina con un GET 'rgt=1' para indicarle al codigo del archivo 'registroCTRL.php o register.php' que se pudo registrar
        } else {
            die(print($pdo->errorInfo()));
        }
    } catch (PDOException $error) {
        header("Location: register.php?rgt=0"); // si se inserta incorrectamente se redirecciona a la pagina un post 'rgt=0' para indicarle al codigo del archivo 'registroCTRL.php o register.php' que no se pudo registrar e intente nuevamente
        echo $error->getMessage();
        die();
    }
    try {
        $pdo = $conexion->prepare('INSERT INTO puntos_usuarios(id, dni_usuario, puntos) VALUES(NULL, ?, 0)'); // sentencia sql para asignarle un espacio al usuario que se acaba de registrar en la tabla de puntajes y el contador se inicie en 0
        $pdo->bindParam(1, $dni); // le pasamos como parámetro el numero de dni
        if ($pdo->execute()) {
            header("Location: register.php?rgt=1"); // si se inserta correctamente se redirecciona a la pagina un post 'rgt=1' para indicarle al codigo del archivo 'registroCTRL.php o register.php' que se pudo registrar
        } else {
            die(print($pdo->errorInfo()));
        }
    } catch (PDOException $error) {
        header("Location: register.php?rgt=0"); // si se inserta incorrectamente se redirecciona a la pagina un post 'rgt=0' para indicarle al codigo del archivo 'registroCTRL.php o register.php' que no se pudo registrar e intente nuevamente
        echo $error->getMessage();
        die();
    }
}
function comp_dni($dni, $conexion)  // definimos la funcion que revisa si un usuario está registrado en la db
{
    try {
        $pdo = $conexion->prepare('SELECT Nombre FROM usuarios WHERE dni = ?'); // sentencia sql que consulta si hay coincidencias con el dni que se pase como parametro
        $pdo->bindParam(1, $dni); // le pasamos como parámetro el numero de dni
        $pdo->execute() or die(print($pdo->errorInfo())); // se ejecuta, si da error se imprime informacion sobre el error
        $resultado = $pdo->fetchAll(PDO::FETCH_ASSOC); // el resultado (con formato array) de la consulta se guarda en una variable
        if ($resultado) {
            return True; // si el resultado es valido (no es vacio, se encontró alguna coincidencia con el dni porque está registrado) se retorna valor booleano True
        } else {
            return False; // si el resultado no es valido (es vacio, no se encontró alguna coincidencia con el dni porque está registrado) se retorna valor booleano False
        }
    } catch (PDOException $error) {
        echo $error->getMessage();
        die();
    }
}
function userData($dni, $conexion) // definimos la funcion que devuelve los datos sobre los usuarios
{
    try {
        $pdo = $conexion->prepare('SELECT * FROM usuarios WHERE dni = ?'); // sentencia sql que recibe los datos del usuario registrado en la db que coincide en el dni con el que se pase como parámetro
        $pdo->bindParam(1, $dni); // le damos el valor de la variable $dni que se pasó como parametro al llamar la funcion
        $pdo->execute() or die(print($pdo->errorInfo()));
        $resultado_user = $pdo->fetchAll(PDO::FETCH_ASSOC); // el resultado (con formato array) de la consulta se guarda en una variable
    } catch (PDOException $error) {
        echo $error->getMessage();
        die();
    }
    try {
        $pdo = $conexion->prepare('SELECT * FROM puntos_usuarios WHERE dni_usuario = ?');
        $pdo->bindParam(1, $dni);
        $pdo->execute() or die(print($pdo->errorInfo()));
        $resultado_puntos = $pdo->fetchAll(PDO::FETCH_ASSOC); // el resultado (con formato array) de la consulta se guarda en una variable
    } catch (PDOException $error) {
        echo $error->getMessage();
        die();
    }
    try {
        $pdo = $conexion->prepare('SELECT Rank FROM (SELECT *, RANK() OVER(ORDER BY puntos DESC) as Rank FROM puntos_usuarios) AS Subquery WHERE dni_usuario = ?');

        
        // sentencia sql que consulta el ranking de usuarios y el correspondiente al usuario solicitado, mediante una subconsulta se obtiene el orden que le da el ranking en base a los puntos
        $pdo->bindParam(1, $dni);  // le damos el valor de la variable $dni que se pasó como parametro al llamar la funcion
        $pdo->execute(); // se ejecuta la consulta
        $resultado_rank = $pdo->fetchAll(PDO::FETCH_ASSOC); // el resultado (con formato array) de la consulta se guarda en una variable
    } catch (PDOException $error) {
        echo $error->getMessage();
        die();
    }
    return array($resultado_user, $resultado_puntos, $resultado_rank); // se retorna en formato array para ser capaz de que las variables de todos los resultados que hubo en cada consulta se puedan utilizar al momento de imprimir los datos del usuarios en la pagina web 'userdata.php'
}
function consultaScanner($producto, $dni, $conexion)
{
    try {
        $pdo = $conexion->prepare('SELECT * FROM productos WHERE code_barras = ?');
        $pdo->bindParam(1, $producto);
        $pdo->execute() or die(print($pdo->errorInfo()));
        $resultado = $pdo->fetchAll(PDO::FETCH_ASSOC); // el resultado (con formato array) de la consulta se guarda en una variable
        if ($resultado) {
            registrarPuntos($resultado[0]['valor_puntos'], $dni, $conexion); // se utiliza el resultado de la consulta ($resultado[0]['valor_puntos']) para asignarle los puntos al usuario mediante la funcion 'registrarPuntos()''
            return $resultado[0]['valor_puntos']; // se retonra le valor de los puntos para que el codigo de arduino de la placa ESP32 sepa cuantos puntos se le asignaron al usuarioaaa
        } else {
            return 0; // en caso de que no haya habido resultado positivo en la consulta (es decir, no se encontró un producto registrado coincidente con el codigo de barras que envió el codigo de arduino), se retorna un '0' para indicarle el resultado de la consulta al arduino
        }
    } catch (PDOException $error) {
        echo $error->getMessage();
        die();
    }
}
function registrarPuntos($puntos, $dni, $conexion) // esta funcion se encarga de sumarle los puntos al usuario que ingrese productos al tacho y recibe como parámetro el valor en puntos del producto escaneado
{
    try {
        $pdo = $conexion->prepare('UPDATE puntos_usuarios SET puntos = puntos + ? WHERE dni_usuario = ? ');
        $pdo->bindParam(1, $puntos); // se utiliza la variable $puntos que lleva el valor en puntos del producto escaneado y se manda como parámetro al UPDATE que se ejecutan en la DB
        $pdo->bindParam(2, $dni);
        $pdo->execute() or die(print($pdo->errorInfo()));
    } catch (PDOException $error) {
        echo $error->getMessage();
        die();
    }
}

/* HUMO DEL INGERNIEROOOOOOOOOOOOOOOO */

function countUsers($conexion) // definimos la función para contar la cantidad de usuarios en la base de datos
{
    try {
        $pdo = $conexion->prepare('SELECT COUNT(*) as total FROM usuarios'); // sentencia SQL para contar el número de usuarios
        $pdo->execute() or die(print($pdo->errorInfo())); // ejecutamos la consulta y si hay un error se imprime la información del error
        $resultado = $pdo->fetch(PDO::FETCH_ASSOC); // obtenemos el resultado de la consulta
        return $resultado['total']; // retornamos la cantidad de usuarios
    } catch (PDOException $error) {
        echo $error->getMessage(); // en caso de error, mostramos el mensaje de error
        die(); // terminamos la ejecución
    }
}

function countProducts($conexion) {
    try {
        $pdo = $conexion->prepare('SELECT COUNT(*) as total FROM productos');
        $pdo->execute() or die(print($pdo->errorInfo()));
        $resultado = $pdo->fetch(PDO::FETCH_ASSOC);
        return $resultado['total'];
    } catch (PDOException $error) {
        echo $error->getMessage();
        die();
    }
}

function countTotalPoints($conexion) {
    try {
        $pdo = $conexion->prepare('SELECT SUM(puntos) as total FROM puntos_usuarios');
        $pdo->execute() or die(print($pdo->errorInfo()));
        $resultado = $pdo->fetch(PDO::FETCH_ASSOC);
        return $resultado['total'];
    } catch (PDOException $error) {
        echo $error->getMessage();
        die();
    }
}

