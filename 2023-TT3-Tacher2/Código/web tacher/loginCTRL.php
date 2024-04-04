<?php
include "database.php";

$dni = isset($_POST['int_dni_usuario']) ? $_POST['int_dni_usuario'] : '';
$conexion = conexion();

/* ----------------------------------------------------------------------------------------------- */

if (comp_dni($dni, $conexion)) {
    session_start();
    $_SESSION['user'] = $dni;
    header("Location: userdata.php");
} else {
    header("Location: login.php?rgt=0");
} 

/* <?php
    $usuario = userData($_SESSION['user'], $conexion)
    ?> */