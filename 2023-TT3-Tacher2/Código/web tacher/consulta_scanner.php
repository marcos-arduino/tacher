<?php



include_once "database.php"; // importo el archivo que ejecuta las sentencias sql

// la linea 13 es lo mismo que hacer:
// if (isset($_POST['user_dni'])) {
//     $dni = $_POST['user_dni'];
// } else {
//     $dni = '';
// }

try{
    $dni = isset($_POST['user_dni']) ? $_POST['user_dni'] : ''; // recibe el dni escaneado en el GM65 conectado a la ESP32
    $producto = isset($_POST['code_bar']) ? $_POST['code_bar'] : ''; // recibe el dni escaneado en el GM65 conectado a la ESP32
    } catch (Exception $e){
    echo "-100";
}
if ($producto == "" && $dni == ""){
    echo "2";
} else {
    $conexion = conexion(); // la variable recibe el retorno de la funcion 'conexion()'
    $resultado_user = consultaScanner($producto, $dni, $conexion); // la funcion consulta si el producto enviado está registrado en la db, y en caso de estarlo retorna el valor del producto dentro del sistema de puntaje
    if ($resultado_user) {  // se revisa si hubo un resultado válido
        echo $resultado_user; // se le retorna al arduino el valor que se recibe de la funcion: 0 en caso de que el producto no esté registrado o el valor en puntos que tiene el producto
    }
    else {
        echo "0"; // en caso de que el dni no esté registrado, se muestra un '0'
    }
}
// se muestra el numero del resultado, para que la ESP32 pueda seguir adelante con su codigo en base a lo que se mostró


?>