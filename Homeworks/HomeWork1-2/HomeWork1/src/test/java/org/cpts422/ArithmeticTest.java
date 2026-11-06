package org.cpts422;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class ArithmeticTest {

    // Task: write normal & robust boundary tests for the methods sum(), difference() and quotient()
    // You may or may not use setup and teardown methods.

    // valid range is [1, 200]

    // normal values
    // 1, 2, 199, 200
    //

    // robust values
    // 0, 201

    // nominal value = 100

    // <editor-fold desc ="Arithmetic Sum Testing">
    //  sum normal boundary value tests 9 test cases
    void sumNormalBoundaryTest() {
        assertEquals(101, Arithmetic.sum(1, 100));
        assertEquals(102, Arithmetic.sum(2, 100));
        assertEquals(299, Arithmetic.sum(199, 100));
        assertEquals(300, Arithmetic.sum(200, 100));

        assertEquals(101, Arithmetic.sum(100, 1));
        assertEquals(102, Arithmetic.sum(100, 2));
        assertEquals(299, Arithmetic.sum(100, 199));
        assertEquals(300, Arithmetic.sum(100, 200));

        assertEquals(200, Arithmetic.sum(100, 100));
    }

    // robust boundary value tests 4 test cases

    void sumRobustBoundaryTest() {
        //normal boundary cases
        assertEquals(101, Arithmetic.sum(1, 100));
        assertEquals(102, Arithmetic.sum(2, 100));
        assertEquals(299, Arithmetic.sum(199, 100));
        assertEquals(300, Arithmetic.sum(200, 100));

        assertEquals(101, Arithmetic.sum(100, 1));
        assertEquals(102, Arithmetic.sum(100, 2));
        assertEquals(299, Arithmetic.sum(100, 199));
        assertEquals(300, Arithmetic.sum(100, 200));

        assertEquals(200, Arithmetic.sum(100, 100));

        //robust boundary cases
        assertEquals(100, Arithmetic.sum(0, 100));
        assertEquals(301, Arithmetic.sum(201, 100));

        assertEquals(100, Arithmetic.sum(100, 0));
        assertEquals(301, Arithmetic.sum(100, 201));
    }

    // </editor-fold>

    //<editor-fold desc="Arithmetic Difference Testing">

    void differenceNormalBoundaryTest() {

        // normal boundary values
        assertEquals(-99, Arithmetic.difference(1, 100));
        assertEquals(-98, Arithmetic.difference(2, 100));
        assertEquals(99, Arithmetic.difference(199, 100));
        assertEquals(100, Arithmetic.difference(200, 100));

        assertEquals(99, Arithmetic.difference(100, 1));
        assertEquals(98, Arithmetic.difference(100, 2));
        assertEquals(-99, Arithmetic.difference(100, 199));
        assertEquals(-100, Arithmetic.difference(100, 200));

        assertEquals(0, Arithmetic.difference(100, 100));
    }

    // robust boundary value tests 13 test cases

    void differenceRobustBoundaryTesting() {

        // normal boundary values
        assertEquals(-99, Arithmetic.difference(1, 100));
        assertEquals(-98, Arithmetic.difference(2, 100));
        assertEquals(99, Arithmetic.difference(199, 100));
        assertEquals(100, Arithmetic.difference(200, 100));

        assertEquals(99, Arithmetic.difference(100, 1));
        assertEquals(98, Arithmetic.difference(100, 2));
        assertEquals(-99, Arithmetic.difference(100, 199));
        assertEquals(-100, Arithmetic.difference(100, 200));

        assertEquals(0, Arithmetic.difference(100, 100));

        // robust boundary values
        assertEquals(-100, Arithmetic.difference(0, 100));
        assertEquals(101, Arithmetic.difference(201, 100));

        assertEquals(100, Arithmetic.difference(100, 0));
        assertEquals(-101, Arithmetic.difference(100, 201));
    }

    //</editor-fold>

    //<editor-fold desc="Arithmetic Quotient Testing">

    void quotientNormalBoundaryTest() {

        // normal boundary values
        assertEquals(0, Arithmetic.quotient(1, 100));
        assertEquals(0, Arithmetic.quotient(2, 100));
        assertEquals(1, Arithmetic.quotient(199, 100));
        assertEquals(2, Arithmetic.quotient(200, 100));

        assertEquals(100, Arithmetic.quotient(100, 1));
        assertEquals(50, Arithmetic.quotient(100, 2));
        assertEquals(0, Arithmetic.quotient(100, 199));
        assertEquals(0, Arithmetic.quotient(100, 200));

        assertEquals(1, Arithmetic.quotient(100, 100));
    }

    // robust boundary value tests 13 test cases

    void quotientRobustBoundaryTesting() {

        // normal boundary values
        assertEquals(0, Arithmetic.quotient(1, 100));
        assertEquals(0, Arithmetic.quotient(2, 100));
        assertEquals(1, Arithmetic.quotient(199, 100));
        assertEquals(2, Arithmetic.quotient(200, 100));

        assertEquals(100, Arithmetic.quotient(100, 1));
        assertEquals(50, Arithmetic.quotient(100, 2));
        assertEquals(0, Arithmetic.quotient(100, 199));
        assertEquals(0, Arithmetic.quotient(100, 200));

        assertEquals(1, Arithmetic.quotient(100, 100));

        // robust boundary values
        assertEquals(0, Arithmetic.quotient(0, 100));
        assertEquals(2, Arithmetic.quotient(201, 100));

        assertThrows(ArithmeticException.class,
                () -> Arithmetic.quotient(100, 0));

        assertEquals(0, Arithmetic.quotient(100, 201));
    }

    //</editor-fold>






}